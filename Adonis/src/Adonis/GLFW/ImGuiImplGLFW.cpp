#include "pch.h"
#include "Adonis/ImGui/ImGuiImplWindow.h"
#include "ImGuiImplGLFW.h"


namespace Adonis {

	void imgui_window_shutdown() {
		ImGui_ImplGlfw_Shutdown();
	}

	void imgui_window_newframe() {
		ImGui_ImplGlfw_NewFrame();
	}

	bool imgui_window_init(GLFWwindow* window, bool install_callbacks) {
		return ImGui_ImplGlfw_InitForOpenGL(window, install_callbacks);
	}

}