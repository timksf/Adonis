#pragma once

#include "Core.h"

namespace Adonis {

	class ADONIS_API Application {
	public:
		Application();
		virtual ~Application();

		auto run()const->void;

	};

	Application* createApplication();

}