//#pragma once
//
//#include "Adonis/Core.h"
//#include "Adonis/Layers/Layer.h"
//#include "Adonis/Eventsystem/Event.h"
//#include "Adonis/Eventsystem/Events/Events.h"
//
//namespace Adonis {
//
//	class ADONIS_API ImGuiLayer : public Layer {
//	public:
//
//		enum class Style {
//			Classic,
//			Dark,
//			Light
//		};
//
//		ImGuiLayer(Style style = Style::Dark);
//
//		auto attach()const->void override;
//		auto detach()const->void override;
//
//		auto on_update(const event::event_ptr_t<event::UpdateEvent>& event)const->void override;
//	private:
//		Style m_style;
//	};
//
//}
