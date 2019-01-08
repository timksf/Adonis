#pragma once

#include "Core.h"
#include "Eventsystem/Event.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"

namespace Adonis {

	class ADONIS_API Application: public event::EventListener {
	public:
		Application();
		virtual ~Application();

		auto run()->void;

		auto onTestEvent(event::event_ptr_t<event::TestEvent>& ev)->void;
		auto on_update_event(event::event_ptr_t<event::UpdateEvent>& ev)->void;
	};

	Application* createApplication();

}