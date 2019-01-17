#include "pch.h"
#include "Application.h"
#include "Eventsystem/EventManager.h"
#include "Eventsystem/EventListener.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Adonis {

	Application* Application::s_instance = nullptr;

	Application::Application(): EventListener(){
		AD_CORE_ASSERT((s_instance == nullptr), "Only one application object can exist");
		s_instance = this;
		m_window = IWindow::create(1280, 720);
	}


	Application::~Application(){
	}

	void Application::run(){
		//Set event handlers
		on_event<UpdateEvent>(std::bind(&Application::on_update_event, this, std::placeholders::_1));
		on_event<WindowCloseEvent>(std::bind(&Application::on_window_close, this, std::placeholders::_1));
		on_event<KeyPressed>(std::bind(&Application::on_key_pressed, this, std::placeholders::_1));

		double time = glfwGetTime();


		//Main action loop
		while (m_running) {

			double deltatime = glfwGetTime() - time;
			time = glfwGetTime();

			EventManager::queueEvent<PreRenderEvent>();
			EventManager::queueEvent<RenderEvent>(deltatime);
			EventManager::queueEvent<PostRenderEvent>();
			EventManager::queueEvent<UpdateEvent>();

			EventManager::processEvents();
		}

	}

	void Application::onTestEvent(event_ptr_t<TestEvent>& ev) {
		AD_CORE_INFO("Application's reaction to TestEvent");
	}

	void Application::on_update_event(event_ptr_t<UpdateEvent>& ev) {
	}

	void Application::on_window_close(event_ptr_t<WindowCloseEvent>& ev) {
		m_window.reset();
		m_running = false;
	}

	void Application::on_key_pressed(event_ptr_t<KeyPressed>& ev) {
		if (ev->code() == GLFW_KEY_F11) {
			m_window->toggle_fullscreen();
		}
	}
}