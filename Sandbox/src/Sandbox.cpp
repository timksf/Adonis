#include "Adonis.h"
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
		layers().push_layer<Adonis::ImGuiLayer>(Adonis::ImGuiLayer::Style::Cherry);
	};
	~Sandbox() {};
};


Adonis::Application* Adonis::createApplication() {
	return new Sandbox();
}
	