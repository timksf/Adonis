#include "Adonis.h"
#include "Adonis/Math/Math.h"
#include "glm/ext.hpp"
#include "imgui.h"
#include <iostream>
#include <memory>

#ifdef ADONIS_PLATFORM_WINDOWS
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

class CustomDebugGUI : public Adonis::DebugGUI {
public:

	CustomDebugGUI(Adonis::DebugGUI::Style s) : Adonis::DebugGUI(s) {};

	void on_RenderEvent(const Adonis::event_ptr_t<Adonis::RenderEvent>& e) override;

};

class Sandbox : public Adonis::Application {
public:
	Sandbox() {
		m_dbg_gui = std::make_unique<CustomDebugGUI>(Adonis::DebugGUI::Style::Cherry);
	};

	inline void on_PreRenderEvent(const Adonis::event_ptr_t<Adonis::PreRenderEvent>& event) override{
		using namespace Adonis::math::literals;
		this->consume_renderer()->clear();
		m_pipe->get_param("view")->set_mat4f(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, z_translation)));
		m_pipe->get_param("projection")->set_mat4f(glm::perspective(45.0_degf, this->consume_renderer()->aspect_ratio(), 0.1f, 100.0f));
	};

	inline void on_RenderEvent(const Adonis::event_ptr_t<Adonis::RenderEvent>& event) override {
		m_pipe->activate();
		m_vao->bind();
		this->consume_renderer()->drawTriangles(0, 3);
	};

	inline void on_PostRenderEvent(const Adonis::event_ptr_t<Adonis::PostRenderEvent>& event) override{
	
	};

	inline void on_AppStartEvent(const Adonis::event_ptr_t<Adonis::AppStartEvent>& event) override {
		using namespace Adonis::render;
		AD_CLIENT_INFO("App started!");
		float vertices[] =
		{ /*pos:*/ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
					0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		};

		auto pos_attr = VertexAttrib::create(0, 0, VertexType::FLOAT, 3 /*floats*/);
		auto col_attr = VertexAttrib::create(1, 3 * sizeof(float), VertexType::FLOAT, 3 /*floats*/);
		m_vbo = VertexBuffer::create(sizeof(vertices), vertices, BufferBit::DYNAMIC_STORAGE | BufferBit::MAP_READ);
		auto attribs = std::vector<std::unique_ptr<VertexAttrib>>();
		attribs.push_back(std::move(pos_attr));
		attribs.push_back(std::move(col_attr));
		auto desc = VertexArrayDesc::create(std::move(attribs), 0, sizeof(float) * 6);
		m_vao = VertexArray::create();
		m_vao->add_buffer(m_vbo->id(), std::move(desc));
		m_pipe = RenderPipeline::test_pipeline_3D();
		m_pipe->get_param("model")->set_mat4f(glm::mat4(1.0f));
		m_pipe->get_param("view")->set_mat4f(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, z_translation)));
		auto projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		m_pipe->get_param("projection")->set_mat4f(glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f));
	};

	~Sandbox() {};

	static float z_translation;
private:
	std::unique_ptr<Adonis::render::RenderPipeline> m_pipe;
	std::unique_ptr<Adonis::render::VertexArray> m_vao;
	std::unique_ptr<Adonis::render::VertexBuffer> m_vbo;
	std::unique_ptr<Adonis::DebugGUI> m_dbg_gui;


};

float Sandbox::z_translation = -1.0f;


void CustomDebugGUI::on_RenderEvent(const Adonis::event_ptr_t<Adonis::RenderEvent>& e) {
	ImGui::SetCurrentContext(this->ctx());
	//ImGui::ShowDemoWindow();
	{

		ImGui::Begin("Debug window");
		ImGui::DragFloat("Z_translation", &Sandbox::z_translation, 0.1f);
		ImGui::End();

	}
}

Adonis::Application* Adonis::create_application() {
	return new Sandbox();
}
	