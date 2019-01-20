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
		ON_EVENT_BIND(KeyPressed,		Application);
		ON_EVENT_BIND(WindowCloseEvent, Application);
		ON_EVENT_BIND(UpdateEvent,		Application);

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


	void Application::on_UpdateEvent(const event_ptr_t<UpdateEvent>& ev) {
	}

	void Application::on_WindowCloseEvent(const event_ptr_t<WindowCloseEvent>& ev) {
		m_window.reset();
		m_running = false;
	}

	void Application::on_KeyPressed(const event_ptr_t<KeyPressed>& ev) {
		if (ev->code() == GLFW_KEY_F11) {
			m_window->toggle_fullscreen();
		}
	}
}