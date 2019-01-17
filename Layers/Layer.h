#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"

namespace Adonis {

	class ADONIS_API Layer : public event::EventListener{
	public:
		
		Layer(const std::string& name);
		virtual ~Layer();

		virtual inline auto name()const->std::string { return m_name; };

		virtual auto attach()const->void = 0;
		virtual auto detach()const->void = 0;
		virtual auto on_update(const event::event_ptr_t<event::UpdateEvent>& ev)const->void = 0;

	private:
		std::string m_name;
	};

}

