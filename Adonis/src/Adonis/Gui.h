#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "imgui.h"


namespace Adonis {

	class ADONIS_API Gui : public EventListener {
	public:

		DECLARE_LISTENER(Gui);

		enum class Style {
			Classic,
			Dark,
			Light,
			Grey,
			Cherry,
			Extasy
		};

		Gui(Style style = Style::Dark);

		virtual ~Gui();

		ON_EVENT_DECL(GuiRenderEvent);
		ON_EVENT_DECL(PreRenderEvent);
		ON_EVENT_DECL(MouseButtonPressed);
		ON_EVENT_DECL(MouseScrolledEvent);
		ON_EVENT_DECL(KeyPressed);
		ON_EVENT_DECL(KeyReleased);
		ON_EVENT_DECL(CharTyped);

		auto ctx()->typename ImGuiContext*;

		auto setup_menu(bool*, bool*, bool*)->void;

		auto show_viewport(bool*)->void;
		auto show_debug(bool*)->void;
		auto show_style_editor(bool*)->void;

	private:
		auto init()->void;
		auto setup_grey_style()const->void;
		auto setup_cherry_style()const->void;
		auto setup_extasy_style()const->void;

		auto update_mouse()->void;
		Style m_style;																					//ImGui Style; Classic, Light or Dark
		float m_time = 0.0;																				//Time used for ImGui updates
		bool m_mousejustpressed[5] = { false, false, false, false, false };
		ImGuiContext* m_ctx = nullptr;



	};

}