#pragma once


#ifdef ADONIS_PLATFORM_WINDOWS

extern Adonis::Application* Adonis::create_application();

int main(int argc, char** argv) {
	Adonis::Log::init();
	AD_CORE_INFO("Initialized loggers");

	auto app = Adonis::create_application();
	app->run();
	delete app;
	return 0;
}

#endif 
