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
			Light
		};

		ImGuiLayer(Style style = Style::Dark);

		auto attach()const->void override;
		auto detach()const->void override;

		auto on_update(const event_ptr_t<UpdateEvent>& event)->void override;
		auto on_pre_render(const event_ptr_t<PreRenderEvent>& event)->void;
		auto on_render(const event_ptr_t<RenderEvent>& event)->void;
		auto on_post_render(const event_ptr_t<PostRenderEvent>& event)->void;
		auto on_mouse_pressed(const event_ptr_t<MouseButtonPressed>& event)->void;
		auto on_scroll(const event_ptr_t<MouseScrolledEvent>& event)->void;
		auto on_key_press(const event_ptr_t<KeyPressed>& event)->void;
		auto on_key_release(const event_ptr_t<KeyReleased>& event)->void;
		auto on_chartyped(const event_ptr_t<CharTyped>& event)->void;

		auto update_mouse()->void;
	private:
		Style m_style;																					//ImGui Style; Classic, Light or Dark
		float m_time = 0.0;																				//Time used for ImGui updates
		bool m_mousejustpressed[5] = { false, false, false, false, false };
	};

}
