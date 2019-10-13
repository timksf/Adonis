#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "glm/glm.hpp"
#include "Adonis/RenderingSystem/RenderingSystem.h"
#include "ViewportWindow.h"
#include <memory>


namespace ImGui {

	static auto vector_getter = [](void* vec, int idx, const char** out_text) {

		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;

	};

}

namespace Adonis {

	class ADONIS_API Gui : public EventListener {
	public:

		DECLARE_LISTENER(Gui);

		enum class Style {
			Classic = 0,
			Dark,
			Light,
			Grey,
			Cherry,
			Extasy,
			LightGreen
		};

		//Gui(const Gui&) = delete;

		//Gui& operator=(const Gui&) = delete;

		Gui(Style style = Style::Dark);

		virtual ~Gui();

		AD_ON_EVENT_DECL(GuiRenderEvent);
		AD_ON_EVENT_DECL(PreRenderEvent);
		AD_ON_EVENT_DECL(MouseButtonPressed);
		AD_ON_EVENT_DECL(MouseScrolledEvent);
		AD_ON_EVENT_DECL(KeyPressed);
		AD_ON_EVENT_DECL(KeyReleased);
		AD_ON_EVENT_DECL(CharTyped);

		auto ctx()->typename ImGuiContext*;

		auto setup_menu(bool*, bool*, bool*, bool*, bool*)->void;

		auto show_viewport(bool*)->void;
		auto show_debug(bool*)->void;
		auto show_style_editor(bool*)->void;
		auto show_tools_window(bool*)->void;
		auto show_scene_edit(bool*)->void;
		auto show_cam_info_window(bool*)->void;;

		inline auto add_viewport_window(std::shared_ptr<gui::ViewportWindow> w)->void { m_viewport_windows.push_back(w); };

	private:
		auto init()->void;
		auto generate_scene_window(std::shared_ptr<rendersystem::Scene>)->void;
		auto setup_grey_style()const->void;
		auto setup_cherry_style()const->void;
		auto setup_extasy_style()const->void;
		auto setup_lightgreen_style()const->void;

		auto update_mouse()->void;
		Style m_style;																					//ImGui Style; Classic, Light or Dark
		float m_time = 0.0;																				//Time used for ImGui updates
		bool m_mousejustpressed[5] = { false, false, false, false, false };
		ImGuiContext* m_ctx = nullptr;
		bool m_esc_pressed = false;
		bool m_in_menu = false;
		bool m_viewport_focused = false;
		bool m_show_cam_info = false;

		std::vector<std::shared_ptr<gui::ViewportWindow>> m_viewport_windows;

	};

	namespace gui {

		class ADONIS_API CustomImGuiElements {
		public:

			static inline bool toggle_button(const char* str_id, bool* v) {

				AD_CORE_ASSERT_NOTNULL(v, "Toggle button cannot be drawn without a valid pointer-to-boolean");

				ImVec2 p = ImGui::GetCursorScreenPos();
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				ImGuiStyle* style = &ImGui::GetStyle();
				auto colors = style->Colors;

				float height = 0.8 * ImGui::GetFrameHeight();
				float width = height * 1.55f;
				float radius = height * 0.50f;

				bool pressed = false;

				pressed = ImGui::InvisibleButton(str_id, ImVec2(width, height));
				if (ImGui::IsItemClicked())
					*v = !*v;

				float t = *v ? 1.0f : 0.0f;

				draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_Button]));

				ImU32 col = *v ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);
				draw_list->AddRectFilled(ImVec2(p.x + t * (width / 2) + 0.05 * width, p.y + 0.1 * height), ImVec2(p.x + t * (width / 2) + width / 2 - 0.05 * width, p.y + 0.9 * height), col);

				if (pressed)
					*v = !*v;

				return pressed;
			}

			static bool Combo(const char* label, int* currIndex, std::vector<std::string>& values){
				if (values.empty()) { return false; }
				return ImGui::Combo(label, currIndex, ImGui::vector_getter,
					static_cast<void*>(&values), values.size());
			}


		};

	}

}

