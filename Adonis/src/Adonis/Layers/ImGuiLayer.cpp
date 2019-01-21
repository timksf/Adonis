#include "pch.h"
#include "ImGuiLayer.h"
#include "Adonis/OpenGL/ImGuiImpl.h"
#include "Adonis/Application.h"

namespace Adonis {

	ImGuiLayer::ImGuiLayer(Style style) : Layer(AD_NAME_FROM_TYPE(ImGuiLayer)), m_style(style) {
		ON_EVENT_BIND(PreRenderEvent,		ImGuiLayer);
		ON_EVENT_BIND(RenderEvent,			ImGuiLayer);
		ON_EVENT_BIND(PostRenderEvent,		ImGuiLayer);
		ON_EVENT_BIND(KeyPressed,			ImGuiLayer);
		ON_EVENT_BIND(KeyReleased,			ImGuiLayer);
		ON_EVENT_BIND(MouseButtonPressed,	ImGuiLayer);
		ON_EVENT_BIND(MouseScrolledEvent,	ImGuiLayer);
		ON_EVENT_BIND(CharTyped,			ImGuiLayer);
		AD_CORE_INFO(__func__);
		attach();
	}

	void ImGuiLayer::on_UpdateEvent(const event_ptr_t<UpdateEvent>& event){

	}

	void ImGuiLayer::attach()const{
		
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
		case Style::Grey:
			setup_grey_style();
			break;
		case Style::Cherry:
			setup_cherry_style();
			break;
		case Style::Extasy:
			setup_extasy_style();
			break;
		}

		// Setup back-end capabilities flags
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
		io.BackendPlatformName = ADONIS_WINDOW_CONTEXT_LIB;

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] =			ADONIS_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] =		ADONIS_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] =	ADONIS_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] =		ADONIS_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] =		ADONIS_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] =		ADONIS_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] =		ADONIS_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] =			ADONIS_KEY_HOME;
		io.KeyMap[ImGuiKey_End] =			ADONIS_KEY_END;
		io.KeyMap[ImGuiKey_Insert] =		ADONIS_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] =		ADONIS_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] =		ADONIS_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] =			ADONIS_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] =			ADONIS_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] =		ADONIS_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] =				ADONIS_KEY_A;
		io.KeyMap[ImGuiKey_C] =				ADONIS_KEY_C;
		io.KeyMap[ImGuiKey_V] =				ADONIS_KEY_V;
		io.KeyMap[ImGuiKey_X] =				ADONIS_KEY_X;
		io.KeyMap[ImGuiKey_Y] =				ADONIS_KEY_Y;
		io.KeyMap[ImGuiKey_Z] =				ADONIS_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::detach()const {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

	}

	void ImGuiLayer::on_PreRenderEvent(const event_ptr_t<PreRenderEvent>& ev) {
	}

	void ImGuiLayer::on_RenderEvent(const event_ptr_t<RenderEvent>& ev) {
		ImGui_ImplOpenGL3_NewFrame();
		update_mouse();

		ImGuiIO& io = ImGui::GetIO();
		Application* app = Application::get();

		float width = static_cast<float>(app->consume_window()->width());
		float height = static_cast<float>(app->consume_window()->height());
		io.DisplaySize = ImVec2(width, height);
		io.DisplayFramebufferScale = ImVec2(width > 0 ? (static_cast<float>(app->consume_window()->framebuffer_width()) / width) : 0, height > 0 ? (static_cast<float>(app->consume_window()->framebuffer_height()) / height) : 0);

		double current_time = app->consume_window()->get_time();
		io.DeltaTime = m_time > 0.0 ? static_cast<float>(current_time - m_time) : static_cast<float>(1.0f / 60.0f);
		m_time = static_cast<float>(current_time);

		ImGui::NewFrame();
		bool show_demo = true;
		ImGui::ShowDemoWindow(&show_demo);

		{
			ImGui::Begin("Debug window");

			static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
			ImGui::ColorEdit4("Clear color", col2);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Checkbox("VSYNC", &app->consume_window()->vsync());
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::on_PostRenderEvent(const event_ptr_t<PostRenderEvent>& ev) {
	}

	void ImGuiLayer::on_MouseButtonPressed(const event_ptr_t<MouseButtonPressed>& event) {
		if (event->button() >= 0 && event->button() < AD_ARRAYSIZE(m_mousejustpressed)) {
			m_mousejustpressed[event->button()] = true;
		}
	}

	void ImGuiLayer::on_MouseScrolledEvent(const event_ptr_t<MouseScrolledEvent>& event) {
		//AD_CORE_INFO("Mouse scrolled event captured by imgui layer: {0}, {1}", event->xoffset(), event->yoffset());
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += static_cast<float>(event->xoffset());
		io.MouseWheel += static_cast<float>(event->yoffset());
	}

	void ImGuiLayer::on_KeyPressed(const event_ptr_t<KeyPressed>& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event->code()] = true;
		io.KeyCtrl	= io.KeysDown[ADONIS_KEY_LEFT_CONTROL]	|| io.KeysDown[ADONIS_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[ADONIS_KEY_LEFT_SHIFT]	|| io.KeysDown[ADONIS_KEY_RIGHT_SHIFT];
		io.KeyAlt	= io.KeysDown[ADONIS_KEY_LEFT_ALT]		|| io.KeysDown[ADONIS_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[ADONIS_KEY_LEFT_SUPER]	|| io.KeysDown[ADONIS_KEY_RIGHT_SUPER];
	}

	void ImGuiLayer::on_KeyReleased(const event_ptr_t<KeyReleased>& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event->code()] = false;
		// Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[ADONIS_KEY_LEFT_CONTROL]	|| io.KeysDown[ADONIS_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[ADONIS_KEY_LEFT_SHIFT]	|| io.KeysDown[ADONIS_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[ADONIS_KEY_LEFT_ALT]		|| io.KeysDown[ADONIS_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[ADONIS_KEY_LEFT_SUPER]	|| io.KeysDown[ADONIS_KEY_RIGHT_SUPER];
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

	void ImGuiLayer::on_CharTyped(const event_ptr_t<CharTyped>& event) {
		ImGuiIO& io = ImGui::GetIO();
		if (event->character() > 0 && event->character() < 0x10000)
			io.AddInputCharacter(event->character());
	}

	void ImGuiLayer::setup_extasy_style()const {
		/* https://www.unknowncheats.me/forum/direct3d/189635-imgui-style-settings.html */
		ImGuiStyle * style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}

	void ImGuiLayer::setup_grey_style() const{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	}

	void ImGuiLayer::setup_cherry_style()const{
		/* https://github.com/ocornut/imgui/issues/707 */

		// cherry colors, 3 intensities
		#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
		#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
		#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
		// backgrounds (@todo: complete with BG_MED, BG_LOW)
		#define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
		// text
		#define TXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

		auto &style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = TXT(0.78f);
		style.Colors[ImGuiCol_TextDisabled] = TXT(0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		style.Colors[ImGuiCol_ChildWindowBg] = BG(0.58f);
		style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = BG(1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
		style.Colors[ImGuiCol_TitleBg] = LOW(1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
		style.Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
		style.Colors[ImGuiCol_ButtonActive] = MED(1.00f);
		style.Colors[ImGuiCol_Header] = MED(0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
		style.Colors[ImGuiCol_HeaderActive] = HI(1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = MED(0.78f);
		style.Colors[ImGuiCol_ColumnActive] = MED(1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
		style.Colors[ImGuiCol_PlotLines] = TXT(0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = TXT(0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
		// [...]
		style.Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);

		style.WindowPadding = ImVec2(6, 4);
		style.WindowRounding = 0.0f;
		style.FramePadding = ImVec2(5, 2);
		style.FrameRounding = 3.0f;
		style.ItemSpacing = ImVec2(7, 1);
		style.ItemInnerSpacing = ImVec2(1, 1);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 6.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 16.0f;
		style.GrabMinSize = 20.0f;
		style.GrabRounding = 2.0f;

		style.WindowTitleAlign.x = 0.50f;

		style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
		style.FrameBorderSize = 0.0f;
		style.WindowBorderSize = 1.0f;
	}
}