#pragma once

#include "Core.h"
#include "Eventsystem/Event.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "Adonis/Window.h"
#include "Adonis/Rendering/RenderDevice.h"
#include "Gui.h"

namespace Adonis {

	/**
	*	@brief This class contains the main loop and manages all involved systems
	*/
	class ADONIS_API Application: public EventListener {
	public:
		DECLARE_LISTENER(Application);

		Application();
		virtual ~Application();

		auto run()->void;

		ON_EVENT_DECL_V(UpdateEvent);
		ON_EVENT_DECL_V(WindowCloseEvent);
		ON_EVENT_DECL_V(KeyPressed);
		ON_EVENT_DECL_V(AppStartEvent) {};
		ON_EVENT_DECL_V(PreRenderEvent) {};
		ON_EVENT_DECL_V(RenderEvent) {};
		ON_EVENT_DECL_V(PostRenderEvent) {};

		inline auto consume_window()const->IWindow* { return m_window.get(); };
		inline auto consume_renderer()const->render::RenderDevice* { return m_renderer.get(); };
		inline auto consume_gui()const->Gui* { return m_gui.get(); };
		inline auto running()->bool& { return m_running; };

		static inline auto get()->Application* { return s_instance; };
	private:
		static Application* s_instance;
		std::unique_ptr<IWindow> m_window;
		std::unique_ptr<render::RenderDevice> m_renderer;
		std::unique_ptr<Gui> m_gui;
		bool m_running{ true };
	};

	std::unique_ptr<Application> create_application();

}