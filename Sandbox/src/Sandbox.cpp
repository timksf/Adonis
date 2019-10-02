#include "Adonis.h"
#include "glm/ext.hpp"
#include "imgui.h"
#include <iostream>
#include <memory>

#ifdef ADONIS_PLATFORM_WINDOWS
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif



class Sandbox : public Adonis::Application {
public:
	Sandbox() {

		using namespace Adonis::math::literals;
		using namespace Adonis::render;
		using namespace Adonis::rendersystem;
		using namespace Adonis;


		m_scene = std::make_shared<Scene>(SceneType::Scene3D);

		auto colortex = Texture2D::create(400, 300);
		colortex->set_param(TextureParameter::MIN_FILTER, TextureParamValue::FILTER_LINEAR);

		//m_depthtex = Adonis::render::Texture2D::create(400, 300, nullptr, Adonis::render::TexturePixelFormatSized::DEPTH16);

		m_framebuffer = Framebuffer::create();
		m_framebuffer->attach(colortex->id(), FramebufferTextureAttachment::COLOR);

		m_viewport_window = std::make_shared<gui::ViewportWindow>(std::move(colortex), gui::ViewportWindowType::MAIN);

		m_default_vao = VertexArray::create(VertexArrayDesc::default_pos_color_desc());

		float width = 10, height = 10, depth = 10;

		float cube_vertices[] = {
			width / 2.0f, -height / 2.0f, depth / 2.0f,	.0f, 1.0f, 1.0f,
			width / 2.0f,  height / 2.0f, depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, height / 2.0f,  depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, -height / 2.0f, depth / 2.0f,	.0f, 1.0f, 1.0f,
			width / 2.0f,  -height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f,
			width / 2.0f,  height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, -height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f
		};

		uint32_t cube_indices[] = {

			0,1,4,
			4,1,5,
			0,2,1,
			0,3,2,
			0,4,6,
			0,3,6,
			1,5,2,
			2,5,7,
			2,3,6,
			2,6,7,
			5,6,7,
			5,4,6

		};

		auto temp_cube_mesh = std::make_unique<Mesh>(cube_vertices, sizeof(cube_vertices), VertexArrayDesc::default_pos_color_desc(), cube_indices, sizeof(cube_indices));

		auto temp_cube_model = std::make_unique<Model>(std::move(temp_cube_mesh));

		m_scene->set_pipe(RenderPipeline::test_pipeline_3D());
		m_scene->add_default_cam(true);
		m_scene->add_model(std::move(temp_cube_model));
		m_scene->enable_cam();

		renderer()->toggle_wireframe();
		
		gui()->add_viewport_window(m_viewport_window);
	};

	~Sandbox() {};

	inline AD_ON_EVENT_DECL_OVERRIDE(RenderEvent) {

		using namespace Adonis::math::literals;
		using namespace Adonis::render;
		using namespace Adonis::rendersystem;
		
		static bool test = true;

		if (test) {

			if (m_viewport_window->resized()) {
				m_framebuffer->attach(m_viewport_window->texture_id(), FramebufferTextureAttachment::COLOR);
				renderer()->set_viewport(0, 0, m_viewport_window->width(), m_viewport_window->height());
				m_scene->set_resolution(m_viewport_window->width(), m_viewport_window->height());
			}

			if (m_viewport_window->active()) {
				window()->disable_cursor();
				m_scene->enable_cam();
			}
			else {
				window()->enable_cursor();
				m_scene->disable_cam();
			}

			m_default_vao->use();

			activate_scene(m_scene);

			renderer()->set_framebuffer(m_framebuffer->id());
			m_framebuffer->activate_color_attachment(0);

			renderer()->clear_color = { {.0f, .0f, .0f, 1.f} };
			renderer()->clear_color_buffer();
			renderer()->clear_depth_buffer(0.f);
			renderer()->draw(m_scene);

			//Adonis::Gui::test(m_viewport_window);

			//Activate default framebuffer so that imgui can render to it
			renderer()->set_framebuffer(DEFAULT_FRAMEBUFFER);

		}
	}

	inline AD_ON_EVENT_DECL_OVERRIDE(ViewportResizeEvent) {

		renderer()->set_viewport(0, 0, event->width(), event->height());
		//	m_scene->set_resolution(event->width(), event->height());
		//m_colortex.reset();
		//m_colortex = Adonis::render::Texture2D::create(event->width(), event->height());

	}

	static float z_translation;
private:

	std::shared_ptr<Adonis::rendersystem::Scene> m_scene;
	std::unique_ptr<Adonis::render::Texture2D> m_depthtex;
	std::unique_ptr<Adonis::render::Framebuffer> m_framebuffer;
	std::unique_ptr<Adonis::render::VertexArray> m_default_vao;
	std::shared_ptr<Adonis::gui::ViewportWindow> m_viewport_window;


};

float Sandbox::z_translation = -1.0f;


std::unique_ptr<Adonis::Application> Adonis::create_application() {
	return std::make_unique<Sandbox>();
}
	