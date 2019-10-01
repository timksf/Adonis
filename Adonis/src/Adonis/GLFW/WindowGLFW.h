#pragma once

#include "pch.h"
#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"

#include "Adonis/Window.h"


namespace Adonis {


	class WindowGLFW: public Window{
	public:
		DECLARE_LISTENER(WindowGLFW);

		WindowGLFW(const uint16_t& width, const uint16_t& height, const std::string& title, const bool& vsync, WindowMode mode);

		virtual ~WindowGLFW();

		AD_ON_EVENT_DECL_V_OVERRIDE(UpdateEvent);
		AD_ON_EVENT_DECL_V_OVERRIDE(PreRenderEvent);
		AD_ON_EVENT_DECL_V_OVERRIDE(RenderEvent);
		AD_ON_EVENT_DECL_V_OVERRIDE(PostRenderEvent);
		AD_ON_EVENT_DECL_V_OVERRIDE(WindowCloseEvent);
		AD_ON_EVENT_DECL_V_OVERRIDE(WindowResizeEvent);

		auto init()->void override;																				
		auto toggle_fullscreen()->void override;																
		auto set_mouse_pos(double x, double y)->void override;
		auto update_vsync()->void override;

		auto toggle_cursor()->void override;
		auto enable_cursor()->void override;
		auto disable_cursor()->void override;

		auto has_focus()const->bool override;
		
		auto is_vsync()const->bool override { return m_vsync; };
		auto vsync()->bool& override;

		inline auto pos()->glm::ivec2 override;
		inline auto set_pos(int x, int y)->void override{ glfwSetWindowPos(m_window.get(), x, y); };
		inline auto button_state(int button)->int override { return glfwGetMouseButton(m_window.get(), button); };
		inline auto mouse_pos()->glm::dvec2 override{ return m_mouse_pos; };
		inline auto width()->uint16_t override { return static_cast<uint16_t>(m_width); };
		inline auto height()->uint16_t override { return static_cast<uint16_t>(m_height); };
		inline auto title()->std::string override { return m_title; };											
		inline auto framebuffer_width()->uint16_t { return static_cast<uint16_t>(m_framebuffer_width); };
		inline auto framebuffer_height()->uint16_t { return static_cast<uint16_t>(m_framebuffer_height); };
		inline auto get_time()->double override{ return glfwGetTime(); };

		static auto shutdown(GLFWwindow* window)->void;															//Clean-up allocated storage									

	private:
		bool m_show_cursor{ true };
		std::string m_title;
		int m_width;
		int m_height;
		int m_framebuffer_width;
		int m_framebuffer_height;
		WindowMode m_mode;
		bool m_vsync;
		glm::dvec2 m_mouse_pos;

		std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)> m_window{ nullptr, shutdown };
	};



}

