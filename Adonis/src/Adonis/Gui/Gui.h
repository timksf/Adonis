#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "imgui.h"
#include "glm/glm.hpp"
#include "Adonis/RenderingSystem/RenderingSystem.h"
#include "ViewportWindow.h"
#include <memory>

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
		auto show_cam_info_window(rendersystem::CamInfo info, bool*)->void;

		static inline auto test(std::shared_ptr<gui::ViewportWindow> w) {

			w->draw();

		};

		inline auto add_viewport_window(std::shared_ptr<gui::ViewportWindow> w)->void {

			m_viewport_windows.push_back(w);

		};

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

}