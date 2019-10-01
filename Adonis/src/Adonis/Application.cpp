#include "pch.h"
#include "Application.h"
#include "Eventsystem/EventManager.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/Math/Math.h"
#include "Config.h"
#include "glm/glm.hpp"
#include <glad/glad.h>


namespace Adonis {

	Application* Application::s_instance = nullptr;

	Application::Application(): EventListener(){
		m_config = std::make_shared<Config>("config.json");

		AD_CORE_ASSERT((s_instance == nullptr), "Only one application object can exist");
		s_instance = this;
		m_window = Window::create((*m_config)["window"]["res"]["w"], (*m_config)["window"]["res"]["h"], (*m_config)["window"]["title"], (*m_config)["window"]["vsync"]);
		m_window->set_pos((*m_config)["window"]["pos"]["x"], (*m_config)["window"]["pos"]["y"]);
		m_renderer = render::RenderDevice::create();
		#ifdef ADONIS_DEBUG
				AD_CORE_INFO("Renderer version: {0}", m_renderer->version());
				AD_CORE_INFO("Render device: {0}", m_renderer->renderer());
				AD_CORE_INFO("Shading language version: {0}", m_renderer->sl_language_version());
				AD_CORE_INFO("Vendor: {0}", m_renderer->vendor());
		#endif;
		m_gui = std::make_unique<Gui>((Gui::Style)(*m_config)["gui"]["theme"]);
	}

	Application::~Application(){
	}

	void Application::run(){
		//Set event handlers
		AD_ON_EVENT_BIND(KeyPressed,		Application);
		AD_ON_EVENT_BIND(WindowCloseEvent,	Application);
		AD_ON_EVENT_BIND(UpdateEvent,		Application);
		AD_ON_EVENT_BIND(AppStartEvent,		Application);
		AD_ON_EVENT_BIND(AppStopEvent,		Application);
		AD_ON_EVENT_BIND(PreRenderEvent,	Application);
		AD_ON_EVENT_BIND(RenderEvent,		Application);
		AD_ON_EVENT_BIND(PostRenderEvent,	Application);

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
			EventManager::queueEvent<UpdateEvent>(deltatime);

			EventManager::processEvents();
		}

	}


	void Application::on_UpdateEvent(const event_ptr_t<UpdateEvent>& ev) {
	}

	void Application::on_WindowCloseEvent(const event_ptr_t<WindowCloseEvent>& ev) {
		cleanup();
	}

	AD_EVENT_FUNC_DEF_HEAD(AppStopEvent, Application) {
		cleanup();
	}

	void Application::on_KeyPressed(const event_ptr_t<KeyPressed>& ev) {
		if (ev->code() == ADONIS_KEY_F11) {
			m_window->toggle_fullscreen();
		}
	}

	void Application::activate_viewport() {

		m_window->disable_cursor();
		m_active_scene->enable_cam();

	}

	void Application::cleanup() {

		(*m_config)["window"]["res"]["w"] = m_window->width();
		(*m_config)["window"]["res"]["h"] = m_window->height();
		(*m_config)["window"]["vsync"] = m_window->vsync();
		(*m_config)["window"]["pos"]["x"] = m_window->pos().x;
		(*m_config)["window"]["pos"]["y"] = m_window->pos().y;
		m_window.reset();
		m_running = false;
	}

	void Application::deactivate_viewport(){

		m_window->enable_cursor();

	}

	void Application::viewport_focus_changed(bool focus) {

		if (focus) {
			this->activate_viewport();
		}
		else {
			this->deactivate_viewport();
		}

	}


}