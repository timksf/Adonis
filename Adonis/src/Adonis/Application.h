#pragma once

#include "pch.h"
#include "Core.h"
#include "Eventsystem/Event.h"
#include "Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "Adonis/Window.h"
#include "Adonis/Rendering/Renderer.h"
#include "Gui/Gui.h"

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

		auto viewport_focus_changed(bool focus)->void;

		AD_ON_EVENT_DECL_V(UpdateEvent);
		AD_ON_EVENT_DECL_V(WindowCloseEvent);
		AD_ON_EVENT_DECL_V(KeyPressed);
		AD_ON_EVENT_DECL_V(AppStartEvent) {};
		AD_ON_EVENT_DECL_V(AppStopEvent);
		AD_ON_EVENT_DECL_V(PreRenderEvent) {};
		AD_ON_EVENT_DECL_V(RenderEvent) {};
		AD_ON_EVENT_DECL_V(PostRenderEvent) {};
		AD_ON_EVENT_DECL_V(ViewportResizeEvent) {};

		inline auto consume_window()const->Window* { return m_window.get(); };
		inline auto consume_renderer()const->render::RenderDevice* { return m_renderer.get(); };
		inline auto consume_gui()const->Gui* { return m_gui.get(); };

		inline auto config()const->std::shared_ptr<nlohmann::json> { return m_config; };

		inline auto running()->bool& { return m_running; };

		static inline auto get()->Application* { return s_instance; };

		inline auto activate_scene(std::shared_ptr<rendersystem::Scene> scene)->void { m_active_scene = scene; };

		inline auto active_scene()->std::shared_ptr<rendersystem::Scene> { return m_active_scene; };

	protected:

		inline auto renderer()const->render::RenderDevice* { return m_renderer.get(); };

		inline auto window()const->Window* { return m_window.get(); };

		inline auto gui()const->Gui* { return m_gui.get(); };


	private:

		auto activate_viewport()->void;
		auto deactivate_viewport()->void;

		auto cleanup()->void;
		
		static Application* s_instance;
		std::unique_ptr<Window> m_window{ nullptr };
		std::unique_ptr<render::RenderDevice> m_renderer{ nullptr };
		std::unique_ptr<Gui> m_gui{ nullptr };
		std::shared_ptr<rendersystem::Scene> m_active_scene{ nullptr };

		std::shared_ptr<nlohmann::json> m_config{ nullptr };
		bool m_running{ true };
	};

	std::unique_ptr<Application> create_application();

}