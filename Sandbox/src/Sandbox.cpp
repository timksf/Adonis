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
	};

	~Sandbox() {};

	static float z_translation;
private:
	std::unique_ptr<Adonis::render::RenderPipeline> m_pipe;
	std::unique_ptr<Adonis::render::VertexArray> m_vao;
	std::unique_ptr<Adonis::render::VertexBuffer> m_vbo;


};

float Sandbox::z_translation = -1.0f;


std::unique_ptr<Adonis::Application> Adonis::create_application() {
	return std::make_unique<Sandbox>();
}
	