#pragma once

#include "Adonis/Core.h"

struct ImDrawData;

namespace Adonis {

	ADONIS_API void imgui_renderer_shutdown();
	ADONIS_API bool imgui_renderer_init(const char* glsl_version = NULL);
	ADONIS_API void imgui_renderer_newframe();
	ADONIS_API void imgui_renderer_renderdata(ImDrawData* draw_data);


}
