#pragma once

#include "Core.h"
#include "Eventsystem/Event.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "Adonis/Window.h"
#include "Adonis/Layers/LayerStack.h"
#include "Adonis/RenderDevice.h"

namespace Adonis {

	class ADONIS_API Application: public EventListener {
	public:
		DECLARE_LISTENER(Application);

		Application();
		virtual ~Application();

		auto run()->void;

		ON_EVENT_DECL(UpdateEvent);
		ON_EVENT_DECL(WindowCloseEvent);
		ON_EVENT_DECL(KeyPressed);
		ON_EVENT_DECL(AppStartEvent);

		inline auto consume_window()const->IWindow* { return m_window.get(); };
		inline auto consume_renderer()const->render::RenderDevice* { return m_renderer.get(); };
		inline auto layers()->LayerStack& { return m_layers; };

		static inline auto get()->Application* { return s_instance; };

	private:
		static Application* s_instance;
		std::unique_ptr<IWindow> m_window;
		std::unique_ptr<render::RenderDevice> m_renderer;
		LayerStack m_layers;
		bool m_running{ true };
	};

	Application* createApplication();

}