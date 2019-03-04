#include "pch.h"
#include "Gui.h"
#include "Adonis/ImGui/ImGuiImplRenderer.h"
#include "Adonis/Application.h"
#include "imgui_internal.h"
#include "glm/ext.hpp"
#include "Math/Math.h"

namespace Adonis {

	Gui::Gui(Style style) : m_style(style) {
		ON_EVENT_BIND(KeyPressed, Gui);
		ON_EVENT_BIND(KeyReleased, Gui);
		ON_EVENT_BIND(MouseButtonPressed, Gui);
		ON_EVENT_BIND(MouseScrolledEvent, Gui);
		ON_EVENT_BIND(CharTyped, Gui);
		ON_EVENT_BIND(GuiRenderEvent, Gui);
		ON_EVENT_BIND(PreRenderEvent, Gui);
		init();
	}

	Gui::~Gui() {
		imgui_renderer_shutdown();
		ImGui::DestroyContext();
	}

	ImGuiContext* Gui::ctx() {
		return m_ctx;
	}

	void Gui::init() {

		m_ctx = ImGui::CreateContext();

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
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.BackendPlatformName = ADONIS_WINDOW_CONTEXT_LIB_NAME;

		/*ImFont* font1 = io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 20);
		io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 30);*/


		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = ADONIS_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = ADONIS_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = ADONIS_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = ADONIS_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = ADONIS_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = ADONIS_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = ADONIS_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = ADONIS_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = ADONIS_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = ADONIS_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = ADONIS_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = ADONIS_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = ADONIS_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = ADONIS_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = ADONIS_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = ADONIS_KEY_A;
		io.KeyMap[ImGuiKey_C] = ADONIS_KEY_C;
		io.KeyMap[ImGuiKey_V] = ADONIS_KEY_V;
		io.KeyMap[ImGuiKey_X] = ADONIS_KEY_X;
		io.KeyMap[ImGuiKey_Y] = ADONIS_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = ADONIS_KEY_Z;

		imgui_renderer_init("#version 450 core");

	}

	void Gui::on_PreRenderEvent(const event_ptr_t<PreRenderEvent>& e) {
		imgui_renderer_newframe();
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
		{

			static bool show_debug_window = true;
			static bool show_imgui_demo = false;
			static bool show_style_chooser = false;
			static bool show_render_window = true;
			static bool show_tools = true;
			static glm::vec2 view_port_res{ 0, 0 }, texture_res{ 0, 0 };
			auto app = Application::get();

			//Setup main docking space
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));


			if (!ImGui::Begin("Adonis", &app->running(), window_flags)) {
				ImGui::End();
				AD_CORE_ERROR("Failed to set up gui");
			}
			else {
				
				ImGui::PopStyleVar();
				ImGui::PopStyleVar(2);

				ImGuiID dockspace_id = ImGui::GetID("Adonis");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

				setup_menu(&show_debug_window, &show_imgui_demo, &show_style_chooser, &show_tools);

				if(show_debug_window) show_debug(&show_debug_window);
				if(show_imgui_demo) ImGui::ShowDemoWindow(&show_imgui_demo);
				if(show_style_chooser) show_style_editor(&show_style_chooser);
				if(show_tools) show_tools_window(&show_tools, &view_port_res, &texture_res);
				//if (show_render_window) show_viewport(nullptr);

				//TEST RENDERING
				using namespace math::literals;
				using namespace render;

				static std::shared_ptr<Texture2D> colortex = Texture2D::create(640, 400);
				static std::shared_ptr<Texture2D> depthtex = Texture2D::create(640, 400, nullptr, TexturePixelFormatSized::DEPTH16);
				static std::shared_ptr<Framebuffer> fb = Framebuffer::create();

				colortex->set_param(TextureParameter::MIN_FILTER, TextureParamValue::FILTER_LINEAR);
				colortex->set_param(TextureParameter::MAG_FILTER, TextureParamValue::FILTER_LINEAR);

				fb->attach(colortex, FramebufferTextureAttachment::COLOR);
				//fb->attach(depthtex, FramebufferTextureAttachment::DEPTH);

				static std::unique_ptr<RenderPipeline> pipe = RenderPipeline::test_pipeline_2D();

				static float vertices[] =
					{ /*pos:*/ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
								0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
					};

				static float z_translation = -10.0f;
				static auto pos_attr = VertexAttrib::create(0, 0, VertexType::FLOAT, 3 /*floats*/);
				static auto col_attr = VertexAttrib::create(1, 3 * sizeof(float), VertexType::FLOAT, 3 /*floats*/);
				static auto vbo = VertexBuffer::create(sizeof(vertices), vertices, BufferBit::DYNAMIC_STORAGE | BufferBit::MAP_READ);
				static auto attribs = std::vector<std::unique_ptr<VertexAttrib>>();
				attribs.push_back(std::move(pos_attr));
				attribs.push_back(std::move(col_attr));
				static auto desc = VertexArrayDesc::create(std::move(attribs), 0, sizeof(float) * 6);
				static auto vao = VertexArray::create();
				vao->add_buffer(vbo->id(), std::move(desc));

				//AD_CORE_INFO(glGetError());

				if (!ImGui::Begin("Viewport")) {
					ImGui::End();
					AD_CORE_ERROR("Failed to create render window");
				}
				else {
					float x0 = ImGui::GetCursorScreenPos().x;
					float y0 = ImGui::GetCursorScreenPos().y;
					float ww = ImGui::GetWindowSize().x;
					float wh = ImGui::GetWindowSize().y;
					view_port_res.x = ww;
					view_port_res.y = wh;
					auto lr = ImVec2(x0 + ww - 2, y0 + wh - 2);
					auto ul = ImVec2(x0 - ImGui::GetStyle().FramePadding[0] * 2 + 2, y0 - ImGui::GetStyle().FramePadding[1] * 2 + 2);
					app->consume_renderer()->set_viewport(0, 0, 640, 400);
					app->consume_renderer()->set_framebuffer(fb->id());
					app->consume_renderer()->clear_color = { {1.0f, 1.0f, 1.0f, 1.0f} };
					app->consume_renderer()->clear_color_buffer();
					pipe->activate();
					vao->bind();
					app->consume_renderer()->drawTriangles(0, 3);
					app->consume_renderer()->set_framebuffer(0);
					ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<uint32_t*>(colortex->id()), ul, lr, { 0, 1 }, { 1, 0 });
					ImGui::End();
				}

				//TEST RENDERING END

				ImGui::End();
			}


		}
	}

	void Gui::show_tools_window(bool* show, glm::vec2* vp_res, glm::vec2* tex_res) {
		if (!ImGui::Begin("Tools", show)) {
			AD_CORE_ERROR("Failed to build Tools window");
			ImGui::End();
		}
		else {
			//ImGui::SameLine();
			//ImGui::Text("%.0fx%.0f", vp_res->x, vp_res->y);

			static bool selected;

			ImGui::Button("Update resolution");
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::EndTooltip();
			}

			ImGui::End();
		}
	}

	void Gui::setup_menu(bool* show_debug, bool* show_demo, bool* show_style_edit, bool* show_tools) {
		if (ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("Menu")) {

				ImGui::MenuItem("Debug", NULL, show_debug);
				ImGui::MenuItem("Demo window", NULL, show_demo);
				ImGui::MenuItem("Style chooser", NULL, show_style_edit);
				ImGui::MenuItem("Tools", NULL, show_tools);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void Gui::show_viewport(bool* show_render_window) {
		if (!ImGui::Begin("Viewport", show_render_window, ImGuiWindowFlags_NoTitleBar)) {
			ImGui::End();
			AD_CORE_ERROR("Failed to create render window");
		}
		else {
			ImGui::End();
		}	
	}

	void Gui::show_debug(bool* show_debug_window) {
		ImGuiIO& io = ImGui::GetIO();
		if (!ImGui::Begin("App info", show_debug_window)) {
			ImGui::End();
			AD_CORE_ERROR("Failed to build app info window");
		}
		else {
			ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
			ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
			ImGui::Checkbox("VSYNC", &Application::get()->consume_window()->vsync()),
			ImGui::End();
		}
	}

	void Gui::show_style_editor(bool* show_style_chooser) {
		if (!ImGui::Begin("Style chooser", show_style_chooser)) {
			ImGui::End();
			AD_CORE_ERROR("Failed to create style chooser");
		}
		else {
			static int style_idx = 0;

			if (ImGui::Combo("Themes", &style_idx, "Light\0Dark\0Classic\0Cherry\0Extasy\0Grey")) {

				switch (style_idx) {
				case 0: ImGui::StyleColorsLight(); break;
				case 1: ImGui::StyleColorsDark(); break;
				case 2: ImGui::StyleColorsClassic(); break;
				case 3: setup_cherry_style(); break;
				case 4: setup_extasy_style(); break;
				case 5: setup_grey_style(); break;
				}

			}

			ImGui::End();
		}
	}



	void Gui::on_GuiRenderEvent(const event_ptr_t<GuiRenderEvent>& e) {
		ImGui::Render();
		imgui_renderer_renderdata(ImGui::GetDrawData());
	}

	void Gui::on_MouseButtonPressed(const event_ptr_t<MouseButtonPressed>& event) {
		if (event->button() >= 0 && event->button() < AD_ARRAYSIZE(m_mousejustpressed)) {
			m_mousejustpressed[event->button()] = true;
		}
	}

	void Gui::on_MouseScrolledEvent(const event_ptr_t<MouseScrolledEvent>& event) {
		//AD_CORE_INFO("Mouse scrolled event captured by imgui layer: {0}, {1}", event->xoffset(), event->yoffset());
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += static_cast<float>(event->xoffset());
		io.MouseWheel += static_cast<float>(event->yoffset());
	}

	void Gui::on_KeyPressed(const event_ptr_t<KeyPressed>& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event->code()] = true;
		io.KeyCtrl = io.KeysDown[ADONIS_KEY_LEFT_CONTROL] || io.KeysDown[ADONIS_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[ADONIS_KEY_LEFT_SHIFT] || io.KeysDown[ADONIS_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[ADONIS_KEY_LEFT_ALT] || io.KeysDown[ADONIS_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[ADONIS_KEY_LEFT_SUPER] || io.KeysDown[ADONIS_KEY_RIGHT_SUPER];
	}

	void Gui::on_KeyReleased(const event_ptr_t<KeyReleased>& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event->code()] = false;
		// Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[ADONIS_KEY_LEFT_CONTROL] || io.KeysDown[ADONIS_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[ADONIS_KEY_LEFT_SHIFT] || io.KeysDown[ADONIS_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[ADONIS_KEY_LEFT_ALT] || io.KeysDown[ADONIS_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[ADONIS_KEY_LEFT_SUPER] || io.KeysDown[ADONIS_KEY_RIGHT_SUPER];
	}

	void Gui::update_mouse() {
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

	void Gui::on_CharTyped(const event_ptr_t<CharTyped>& event) {
		ImGuiIO& io = ImGui::GetIO();
		if (event->character() > 0 && event->character() < 0x10000)
			io.AddInputCharacter(event->character());
	}

	void Gui::setup_extasy_style()const {
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

	void Gui::setup_grey_style() const {
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

	void Gui::setup_cherry_style()const {
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