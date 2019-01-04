#pragma once


#ifdef ADONIS_PLATFORM_WINDOWS

extern Adonis::Application* Adonis::createApplication();

int main(int argc, char** agrc) {
	auto app = Adonis::createApplication();
	app->run();
	delete app;
	return 0;
}

#endif 
