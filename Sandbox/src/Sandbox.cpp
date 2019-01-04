#include <Adonis.h>

class Sandbox : public Adonis::Application {
public:
	Sandbox() {};
	~Sandbox() {};
};

Adonis::Application* Adonis::createApplication() {
	AD_CLIENT_INFO("MOIN");
	AD_CLIENT_INFO("MOIN");
	return new Sandbox();
}