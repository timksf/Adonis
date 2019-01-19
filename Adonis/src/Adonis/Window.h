#pragma once

#include "pch.h"
#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include <string>
#include <cstdint>
#include <glm/glm.hpp>


namespace Adonis {

	enum class WindowMode {
		Windowed,
		Borderless,
		Fullscreen
	};

	class ADONIS_API IWindow: public EventListener{
	public:

		static std::unique_ptr<IWindow> create(const uint16_t& width = 640, const uint16_t& height = 480, const std::string& title = "Window", WindowMode mode = WindowMode::Windowed, const bool& vsnyc = true);

		virtual ~IWindow() {};

		virtual auto on_update(const event_ptr_t<UpdateEvent>& ev)->void = 0;			//Defines what should be executed on app's update event
		virtual auto on_pre_render(const event_ptr_t<PreRenderEvent>& ev)->void = 0;
		virtual auto on_render(const event_ptr_t<RenderEvent>& ev)->void = 0;
		virtual auto on_post_render(const event_ptr_t<PostRenderEvent>& ev)->void = 0;
		virtual auto on_exit(const event_ptr_t<WindowCloseEvent>& ev)->void = 0;
		virtual auto init()->void = 0;																//Init underlying window
		virtual auto toggle_fullscreen()->void = 0;													//Toggle fullscreen mode	
		virtual auto set_mouse_pos(double x, double y)->void = 0;

		virtual auto has_focus()const->bool = 0;

		virtual auto is_vsync()const->bool = 0;
									
		virtual auto button_state(int button)->int = 0;
		virtual auto mouse_pos()->glm::dvec2 = 0;
		virtual auto width()->uint16_t = 0;
		virtual auto height()->uint16_t = 0;														//Get width of window
		virtual auto title()->std::string = 0;														//Get height of window		
		virtual auto framebuffer_width()->uint16_t = 0;												//Get title of window
		virtual auto framebuffer_height()->uint16_t = 0;											//Get width of framebuffer
																									//Get height of framebuffer

	};

}