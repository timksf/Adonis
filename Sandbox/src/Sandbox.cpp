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
		auto colortex2 = Texture2D::create(400, 300);
		colortex->set_param(TextureParameter::MIN_FILTER, TextureParamValue::FILTER_LINEAR);
		colortex2->set_param(TextureParameter::MIN_FILTER, TextureParamValue::FILTER_LINEAR);

		//m_depthtex = Adonis::render::Texture2D::create(400, 300, nullptr, Adonis::render::TexturePixelFormatSized::DEPTH16);

		m_framebuffer = Framebuffer::create();

		m_viewport_window = std::make_shared<gui::ViewportWindow>(std::move(colortex), gui::ViewportWindowType::MAIN);
		m_test_viewport = std::make_shared<gui::ViewportWindow>(std::move(colortex2), gui::ViewportWindowType::DEBUG);

		m_default_vao = VertexArray::create(VertexArrayDesc::default_pos_color_desc());

		auto test_cuboid = std::make_unique<primitives::Cuboid>(10, 10, 10);
		auto test_icosahedron = std::make_unique<primitives::Icosahedron>(10);

		auto test_cuboid_model = std::make_shared<Model>(std::move(test_cuboid), "Cube");
		auto test_icosahedron_model = std::make_shared<Model>(std::move(test_icosahedron), "Icosphere");

		m_scene->set_pipe(RenderPipeline::test_pipeline_3D_pos());
		m_scene->add_default_cam(true);
		m_scene->add_model(test_cuboid_model);
		m_scene->add_model(test_icosahedron_model);
		m_scene->enable_cam();

		renderer()->toggle_wireframe();
		
		gui()->add_viewport_window(m_viewport_window);
		gui()->add_viewport_window(m_test_viewport);
	};

	~Sandbox() {};

	inline AD_ON_EVENT_DECL_OVERRIDE(RenderEvent) {

		using namespace Adonis::math::literals;
		using namespace Adonis::render;
		using namespace Adonis::rendersystem;
		
		static bool test = true;

		if (test) {

			if (m_viewport_window->resized()) {
				auto width = m_viewport_window->width();
				auto height = m_viewport_window->height();
				renderer()->set_viewport(0, 0, width, height);
				m_framebuffer->attach(m_viewport_window->texture_id(), FramebufferTextureAttachment::COLOR);
				m_scene->set_resolution(width, height);
				m_test_viewport->set_texture_size(width, height);
				m_framebuffer->attach(m_test_viewport->texture_id(), FramebufferTextureAttachment::COLOR, 1);
			}

			if (m_viewport_window->active()) {
				window()->disable_cursor();
				m_scene->enable_cam();
			}
			else {
				window()->enable_cursor();
				m_scene->disable_cam();
			}
			renderer()->clear_color = { {.0f, .0f, .0f, 1.f} };

			m_default_vao->use();

			activate_scene(m_scene);

			renderer()->set_framebuffer(m_framebuffer->id());

			m_framebuffer->activate_color_attachment(0);
			renderer()->clear_color_buffer();
			//renderer()->clear_depth_buffer(0.f);
			renderer()->draw(m_scene);

			m_framebuffer->activate_color_attachment(1);
			renderer()->clear_color = { {.0f, .0f, .3f, .2f} };
			renderer()->clear_color_buffer();
			//renderer()->clear_depth_buffer(0.f);
			renderer()->draw(m_scene);

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
	std::shared_ptr<Adonis::gui::ViewportWindow> m_test_viewport;


};

float Sandbox::z_translation = -1.0f;


std::unique_ptr<Adonis::Application> Adonis::create_application() {
	return std::make_unique<Sandbox>();
}
	