#pragma once

#include "Adonis/Core.h"

namespace Adonis {

	ADONIS_API void imgui_window_shutdown();
	ADONIS_API void imgui_window_newframe();
	ADONIS_API bool imgui_window_init(GLFWwindow* window, bool install_callbacks);

}