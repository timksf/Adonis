#pragma once

#include "pch.h"
#include "Adonis/Core.h"
#include "Adonis/Window.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"


namespace Adonis {
	
	class WindowsWindow: public IWindow{
	public:
		WindowsWindow(const uint16_t& width, const uint16_t& height, const std::string& title, WindowMode mode, const bool& vsync);

		virtual ~WindowsWindow();

		auto on_update(event::event_ptr_t<event::UpdateEvent>& ev)->void override;
		auto init()->void override;
		auto toggle_fullscreen()->void override;

		inline auto width()->uint16_t override { return m_width; };
		inline auto height()->uint16_t override { return m_height; };
		inline auto title()->std::string override { return m_title; };


	private:
		std::string m_title;
		uint16_t m_width;
		uint16_t m_height;
		WindowMode m_mode;
		bool m_vsync;

		std::unique_ptr<GLFWwindow, void(*)(GLFWwindow *)> m_window{ nullptr, glfwDestroyWindow };
	};



}

