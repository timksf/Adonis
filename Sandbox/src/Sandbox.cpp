#include <Adonis.h>

class Sandbox : public Adonis::Application {
public:
	Sandbox() {};
	~Sandbox() {};
};

Adonis::Application* Adonis::createApplication() {
	return new Sandbox();
}