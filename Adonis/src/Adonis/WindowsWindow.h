#pragma once

#include "pch.h"
#include <glad/glad.h>
#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"

#include "Adonis/Window.h"
#include <GLFW/glfw3.h>

namespace Adonis {


	class WindowsWindow: public IWindow{
	public:
		DECLARE_LISTENER(WindowsWindow);

		WindowsWindow(const uint16_t& width, const uint16_t& height, const std::string& title, WindowMode mode, const bool& vsync);

		virtual ~WindowsWindow();

		auto on_update(const event_ptr_t<UpdateEvent>& ev)->void override;						
		auto on_exit(const event_ptr_t<WindowCloseEvent>& ev)->void override;			
		virtual auto on_pre_render(const event_ptr_t<PreRenderEvent>& ev)->void override;
		virtual auto on_render(const event_ptr_t<RenderEvent>& ev)->void override;
		virtual auto on_post_render(const event_ptr_t<PostRenderEvent>& ev)->void override;
		auto init()->void override;																				
		auto toggle_fullscreen()->void override;																
		static auto shutdown(GLFWwindow* window)->void;															//Clean-up allocated storage									

		inline auto width()->uint16_t override { return static_cast<uint16_t>(m_width); };
		inline auto height()->uint16_t override { return static_cast<uint16_t>(m_height); };
		inline auto title()->std::string override { return m_title; };											
		inline auto framebuffer_width()->uint16_t { return static_cast<uint16_t>(m_framebuffer_width); };
		inline auto framebuffer_height()->uint16_t { return static_cast<uint16_t>(m_framebuffer_height); };


	private:
		std::string m_title;
		int m_width;
		int m_height;
		int m_framebuffer_width;
		int m_framebuffer_height;
		WindowMode m_mode;
		bool m_vsync;

		std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)> m_window{ nullptr, shutdown };
	};



}

