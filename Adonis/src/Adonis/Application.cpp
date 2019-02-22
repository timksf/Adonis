#include "pch.h"
#include "Application.h"
#include "Eventsystem/EventManager.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/OpenGL/GLRenderer.h"
#include "Adonis/Math/Math.h"
#include "glm/glm.hpp"
#include <glad/glad.h>


namespace Adonis {

	Application* Application::s_instance = nullptr;

	Application::Application(): EventListener(){
		AD_CORE_ASSERT((s_instance == nullptr), "Only one application object can exist");
		s_instance = this;
		m_window = IWindow::create(1280, 720);
		EventManager::queueEvent<WindowResizeEvent>(1280, 720);
		m_renderer = render::RenderDevice::create();
		#ifdef ADONIS_DEBUG
				AD_CORE_INFO("Renderer version: {0}", m_renderer->version());
				AD_CORE_INFO("Render device: {0}", m_renderer->renderer());
				AD_CORE_INFO("Shading language version: {0}", m_renderer->sl_language_version());
				AD_CORE_INFO("Vendor: {0}", m_renderer->vendor());
		#endif
	}

	Application::~Application(){
	}

	void Application::run(){
		//Set event handlers
		ON_EVENT_BIND(KeyPressed,		Application);
		ON_EVENT_BIND(WindowCloseEvent, Application);
		ON_EVENT_BIND(UpdateEvent,		Application);
		ON_EVENT_BIND(AppStartEvent,	Application);
		ON_EVENT_BIND(PreRenderEvent,	Application);
		ON_EVENT_BIND(RenderEvent,		Application);
		ON_EVENT_BIND(PostRenderEvent,	Application);

		double time = glfwGetTime();

		EventManager::queueEvent<AppStartEvent>();

		//Main action loop
		while (m_running) {

			double deltatime = glfwGetTime() - time;
			time = glfwGetTime();

			EventManager::queueEvent<PreRenderEvent>();
			EventManager::queueEvent<RenderEvent>(deltatime);
			EventManager::queueEvent<GuiRenderEvent>();
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

}