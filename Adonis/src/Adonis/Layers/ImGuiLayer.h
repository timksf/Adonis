#pragma once

#include "Adonis/Core.h"
#include "Adonis/Layers/Layer.h"
#include "Adonis/Eventsystem/Event.h"
#include "Adonis/Eventsystem/Events/Events.h"

namespace Adonis {

	class ADONIS_API ImGuiLayer : public Layer {
	public:

		DECLARE_LISTENER(ImGuiLayer);

		enum class Style {
			Classic,
			Dark,
			Light,
			Grey,
			Cherry,
			Extasy
		};

		ImGuiLayer(Style style = Style::Dark);

		auto attach()const->void override;
		auto detach()const->void override;

		ON_EVENT_DECL_OVERRIDE(UpdateEvent);
		ON_EVENT_DECL(PreRenderEvent);
		ON_EVENT_DECL(RenderEvent);
		ON_EVENT_DECL(PostRenderEvent);
		ON_EVENT_DECL(MouseButtonPressed);
		ON_EVENT_DECL(MouseScrolledEvent);
		ON_EVENT_DECL(KeyPressed);
		ON_EVENT_DECL(KeyReleased);
		ON_EVENT_DECL(CharTyped);

		auto setup_grey_style()const->void;
		auto setup_cherry_style()const->void;
		auto setup_extasy_style()const->void;

		auto update_mouse()->void;
	private:
		Style m_style;																					//ImGui Style; Classic, Light or Dark
		float m_time = 0.0;																				//Time used for ImGui updates
		bool m_mousejustpressed[5] = { false, false, false, false, false };
	};

}
