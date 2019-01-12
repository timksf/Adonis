#pragma once

#include "Core.h"
#include "Eventsystem/Event.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "Adonis/Window.h"

namespace Adonis {

	class ADONIS_API Application: public event::EventListener {
	public:
		Application();
		virtual ~Application();

		auto run()->void;

		auto onTestEvent(event::event_ptr_t<event::TestEvent>& ev)->void;
		auto on_update_event(event::event_ptr_t<event::UpdateEvent>& ev)->void;
		auto on_window_close(event::event_ptr_t<event::WindowCloseEvent>& ev)->void;
		auto on_key_pressed(event::event_ptr_t<event::KeyPressed>& ev)->void;
	private:
		std::unique_ptr<IWindow> m_window;
		bool m_running{ true };
	};

	Application* createApplication();

}