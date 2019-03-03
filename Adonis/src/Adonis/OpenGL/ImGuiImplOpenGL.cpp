#include "pch.h"
#include "ImGuiImplOpenGL.h"

namespace Adonis {

	void imgui_renderer_shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
	}

	bool imgui_renderer_init(const char* glsl_version) {
		return ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void imgui_renderer_newframe() {
		ImGui_ImplOpenGL3_NewFrame();
	}

	void imgui_renderer_renderdata(ImDrawData* draw_data) {
		ImGui_ImplOpenGL3_RenderDrawData(draw_data);
	}

}