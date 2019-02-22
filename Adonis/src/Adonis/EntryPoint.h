#pragma once


#ifdef ADONIS_PLATFORM_WINDOWS

extern std::unique_ptr<Adonis::Application> Adonis::create_application();

int main(int argc, char** argv) {
	Adonis::Log::init();

	auto app = Adonis::create_application();
	app->run();

	return 0;
}

#endif 
