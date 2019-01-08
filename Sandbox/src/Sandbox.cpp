#include <Adonis.h>
#include <iostream>
#include <memory>

class Sandbox : public Adonis::Application {
public:
	Sandbox() {};
	~Sandbox() {};
};


Adonis::Application* Adonis::createApplication() {
	return new Sandbox();
}