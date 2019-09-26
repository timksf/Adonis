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

		m_scene = std::make_shared<Scene>(SceneType::Scene3D);

		m_colortex = Texture2D::create(400, 300);
		m_colortex->set_param(TextureParameter::MIN_FILTER, TextureParamValue::FILTER_LINEAR);

		//m_depthtex = Adonis::render::Texture2D::create(400, 300, nullptr, Adonis::render::TexturePixelFormatSized::DEPTH16);

		m_framebuffer = Framebuffer::create();
		m_framebuffer->attach(m_colortex->id(), FramebufferTextureAttachment::COLOR);

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
			0u,1u,2u,
			0u,2u,3u,
			1u,4u,5u,
			1u,5u,2u,
			6u,0u,3u,
			6u,3u,7u,
			6u,4u,5u,
			6u,5u,7u,
			3u,2u,5u,
			3u,5u,7u,
			0u,1u,4u,
			0u,4u,6u
		};

		auto temp_cube_mesh = std::make_unique<Mesh>(cube_vertices, sizeof(cube_vertices), VertexArrayDesc::default_pos_color_desc(), cube_indices, sizeof(cube_indices));

		auto temp_cube_model = std::make_unique<Model>(std::move(temp_cube_mesh));

		m_scene->set_pipe(RenderPipeline::test_pipeline_3D());
		//AD_CLIENT_INFO("Camera address from sandbox constructor: {0}", (uint32_t)(temp_xcam.get());
		m_scene->add_default_cam(true);
		//AD_CLIENT_INFO("CAMS: {0}", m_scene->number_of_cams());
		m_scene->add_model(std::move(temp_cube_model));
		m_scene->enable_cam();
		
	};

	~Sandbox() {};

	inline AD_ON_EVENT_DECL_OVERRIDE(RenderEvent) {

		using namespace Adonis::math::literals;
		using namespace Adonis::render;
		using namespace Adonis::rendersystem;

		//m_default_vao->use();

		//activate_scene(m_scene);

		//renderer()->set_framebuffer(m_framebuffer->id());

		//m_framebuffer->activate_color_attachment(0);
		/*renderer()->clear_color = { {.0f, .0f, .0f, 1.f} };
		renderer()->clear_color_buffer();
		renderer()->clear_depth_buffer(0.f);
		renderer()->draw(m_scene);*/

		//Activate default framebuffer so that imgui can render to it
		//renderer()->set_framebuffer(DEFAULT_FRAMEBUFFER);

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
	std::unique_ptr<Adonis::render::Texture2D> m_colortex;
	std::unique_ptr<Adonis::render::Texture2D> m_depthtex;
	std::unique_ptr<Adonis::render::Framebuffer> m_framebuffer;
	std::unique_ptr<Adonis::render::VertexArray> m_default_vao;


};

float Sandbox::z_translation = -1.0f;


std::unique_ptr<Adonis::Application> Adonis::create_application() {
	return std::make_unique<Sandbox>();
}
	