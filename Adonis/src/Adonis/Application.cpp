#include "pch.h"
#include "Application.h"
#include "Eventsystem/EventManager.h"
#include "Eventsystem/EventListener.h"


namespace Adonis {

	Application::Application(): event::EventListener(){
		m_window = IWindow::create();
	}


	Application::~Application(){
	}

	void Application::run(){
		on_event<event::UpdateEvent>(std::bind(&Application::on_update_event, this, std::placeholders::_1));
		on_event<event::WindowCloseEvent>(std::bind(&Application::on_window_close, this, std::placeholders::_1));
		on_event<event::KeyPressed>(std::bind(&Application::on_key_pressed, this, std::placeholders::_1));
		while (m_running) {
			event::EventManager::queueEvent<event::UpdateEvent>();
			event::EventManager::processEvents();
		}
	}

	void Application::onTestEvent(event::event_ptr_t<event::TestEvent>& ev) {
		AD_CORE_INFO("Application's reaction to TestEvent");
	}

	void Application::on_update_event(event::event_ptr_t<event::UpdateEvent>& ev) {
		#ifdef ADONIS_DEBUG
		#endif // ADONIS_DEBUG
	}

	void Application::on_window_close(event::event_ptr_t<event::WindowCloseEvent>& ev) {
		m_running = false;
	}

	void Application::on_key_pressed(event::event_ptr_t<event::KeyPressed>& ev) {
		if (ev->code() == GLFW_KEY_F11) {
			m_window->toggle_fullscreen();
		}
	}
}