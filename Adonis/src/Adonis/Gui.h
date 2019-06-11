#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "imgui.h"
#include "glm/glm.hpp"
#include "Adonis/RenderingSystem/RenderingSystem.h"

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

		auto setup_menu(bool*, bool*, bool*, bool*)->void;

		auto show_viewport(bool*)->void;
		auto show_debug(bool*)->void;
		auto show_style_editor(bool*)->void;
		auto show_tools_window(bool*, glm::vec2*, glm::vec2*, bool*)->void;
		auto show_scene_info(bool*)->void;
		auto show_cam_info_window(rendersystem::CamInfo info, bool*)->void;

	private:
		auto init()->void;
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

	};

}