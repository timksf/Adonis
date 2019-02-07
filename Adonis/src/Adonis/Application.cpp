#include "pch.h"
#include "Application.h"
#include "Eventsystem/EventManager.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/OpenGL/GLRenderer.h"
#include <glad/glad.h>


namespace Adonis {

	Application* Application::s_instance = nullptr;

	Application::Application(): EventListener(){
		AD_CORE_ASSERT((s_instance == nullptr), "Only one application object can exist");
		s_instance = this;
		m_window = IWindow::create(1280, 720);
		m_renderer = render::RenderDevice::create();
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
		if (ev->code() == ADONIS_KEY_F11) {
			m_window->toggle_fullscreen();
		}
	}

	void Application::on_AppStartEvent(const event_ptr_t<AppStartEvent>& ev) {
		#ifdef ADONIS_DEBUG
				AD_CORE_INFO(m_renderer->sl_language_version());
				AD_CORE_INFO(m_renderer->renderer());
				AD_CORE_INFO(m_renderer->version());
		#endif
	}

}