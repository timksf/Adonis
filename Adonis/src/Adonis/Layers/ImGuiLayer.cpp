#include "pch.h"
#include "ImGuiLayer.h"
#include "Adonis/OpenGL/ImGuiImpl.h"
#include "Adonis/OpenGL/ImGuiImplGLFW.h"
#include "Adonis/Application.h"
#include <GLFW/glfw3.h>

namespace Adonis {

	ImGuiLayer::ImGuiLayer(Style style) : Layer(AD_NAME_FROM_TYPE(ImGuiLayer)), m_style(style) {
		on_event<PreRenderEvent>(std::bind(&ImGuiLayer::on_pre_render, this, std::placeholders::_1));
		on_event<RenderEvent>(std::bind(&ImGuiLayer::on_render, this, std::placeholders::_1));
		on_event<PostRenderEvent>(std::bind(&ImGuiLayer::on_post_render, this, std::placeholders::_1));
		on_event<KeyPressed>(std::bind(&ImGuiLayer::on_key_press, this, std::placeholders::_1));
		on_event<KeyReleased>(std::bind(&ImGuiLayer::on_key_release, this, std::placeholders::_1));
		on_event<MouseButtonPressed>(std::bind(&ImGuiLayer::on_mouse_pressed, this, std::placeholders::_1));
		on_event<MouseScrolledEvent>(std::bind(&ImGuiLayer::on_scroll, this, std::placeholders::_1));
		on_event<CharTyped>(std::bind(&ImGuiLayer::on_chartyped, this, std::placeholders::_1));
		attach();
	}

	void ImGuiLayer::on_update(const event_ptr_t<UpdateEvent>& event){

	}

	void ImGuiLayer::attach()const {
		
		ImGui::CreateContext();
		
		switch (m_style) {
		case Style::Dark:
			ImGui::StyleColorsDark();
			break;
		case Style::Light:
			ImGui::StyleColorsLight();
			break;
		case Style::Classic:
			ImGui::StyleColorsClassic();
			break;
		default: break;
		}

		// Setup back-end capabilities flags
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
		io.BackendPlatformName = "imgui_impl_glfw";

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");


	}

	void ImGuiLayer::detach()const {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

	}

	void ImGuiLayer::on_pre_render(const event_ptr_t<PreRenderEvent>& ev) {
	}

	void ImGuiLayer::on_render(const event_ptr_t<RenderEvent>& ev) {
		ImGui_ImplOpenGL3_NewFrame();
		update_mouse();

		ImGuiIO& io = ImGui::GetIO();
		Application* app = Application::get();

		float width = static_cast<float>(app->consume_window()->width());
		float height = static_cast<float>(app->consume_window()->height());
		io.DisplaySize = ImVec2(width, height);
		io.DisplayFramebufferScale = ImVec2(width > 0 ? (static_cast<float>(app->consume_window()->framebuffer_width()) / width) : 0, height > 0 ? (static_cast<float>(app->consume_window()->framebuffer_height()) / height) : 0);

		double current_time = glfwGetTime();
		io.DeltaTime = m_time > 0.0 ? static_cast<float>(current_time - m_time) : static_cast<float>(1.0f / 60.0f);
		m_time = static_cast<float>(current_time);

		ImGui::NewFrame();

		{
			ImGui::Begin("Debug window");

			static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
			ImGui::ColorEdit4("Clear color", col2);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("VSYNC status: %d", app->consume_window()->is_vsync());
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::on_post_render(const event_ptr_t<PostRenderEvent>& ev) {
	}

	void ImGuiLayer::on_mouse_pressed(const event_ptr_t<MouseButtonPressed>& event) {
		if (event->button() >= 0 && event->button() < AD_ARRAYSIZE(m_mousejustpressed)) {
			m_mousejustpressed[event->button()] = true;
		}
	}

	void ImGuiLayer::on_scroll(const event_ptr_t<MouseScrolledEvent>& event) {
		//AD_CORE_INFO("Mouse scrolled event captured by imgui layer: {0}, {1}", event->xoffset(), event->yoffset());
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += static_cast<float>(event->xoffset());
		io.MouseWheel += static_cast<float>(event->yoffset());
	}

	void ImGuiLayer::on_key_press(const event_ptr_t<KeyPressed>& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event->code()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	}

	void ImGuiLayer::on_key_release(const event_ptr_t<KeyReleased>& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event->code()] = false;
		// Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	}

	void ImGuiLayer::update_mouse() {
		ImGuiIO& io = ImGui::GetIO();
		Application* app = Application::get();

		for (size_t i = 0; i < AD_ARRAYSIZE(io.MouseDown); i++) {
			io.MouseDown[i] = m_mousejustpressed[i] || app->consume_window()->button_state(static_cast<int>(i)) != 0;
			m_mousejustpressed[i] = false;
		}

		auto mouse_pos_tmp = io.MousePos;
		io.MousePos = ImVec2(-FLT_MAX, FLT_MAX);

		bool focused = app->consume_window()->has_focus();

		if (focused)
		{
			if (io.WantSetMousePos)
			{
				app->consume_window()->set_mouse_pos(static_cast<double>(mouse_pos_tmp.x), static_cast<double>(mouse_pos_tmp.x));
			}
			else
			{
				io.MousePos.x = static_cast<float>(app->consume_window()->mouse_pos().x);
				io.MousePos.y = static_cast<float>(app->consume_window()->mouse_pos().y);
			}
		}
	}

	void ImGuiLayer::on_chartyped(const event_ptr_t<CharTyped>& event) {
		ImGuiIO& io = ImGui::GetIO();
		if (event->character() > 0 && event->character() < 0x10000)
			io.AddInputCharacter(event->character());
	}
}