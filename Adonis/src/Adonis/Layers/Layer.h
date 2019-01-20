#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"

namespace Adonis {

	class ADONIS_API Layer : public EventListener{
	public:
		
		Layer(const std::string& name);
		virtual ~Layer();

		virtual inline auto name()const->std::string { return m_name; };

		virtual auto attach()const->void = 0;
		virtual auto detach()const->void = 0;
		ON_EVENT_DECL_P_V(UpdateEvent);

	private:
		std::string m_name;
	};

}

