#include "pch.h"
#include "Application.h"
#include "Eventsystem/EventManager.h"
#include "Eventsystem/EventListener.h"


namespace Adonis {

	Application::Application(): event::EventListener(){
	}


	Application::~Application(){
	}

	void Application::run(){
		init<event::UpdateEvent>(std::bind(&Application::on_update_event, this, std::placeholders::_1));
		while (true) {
			event::EventManager::queueEvent< event::UpdateEvent>();
			event::EventManager::processEvents();
		}
	}

	void Application::onTestEvent(event::event_ptr_t<event::TestEvent>& ev) {
		AD_CORE_INFO("Application's reaction to TestEvent");
	}

	void Application::on_update_event(event::event_ptr_t<event::UpdateEvent>& ev) {
		#ifdef ADONIS_DEBUG
			AD_CORE_INFO("App update");
		#endif // ADONIS_DEBUG
	}
}