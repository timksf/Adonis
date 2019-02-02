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

		ON_EVENT_DECL_P_V(UpdateEvent);
		ON_EVENT_DECL_P_V(PreRenderEvent);
		ON_EVENT_DECL_P_V(RenderEvent);
		ON_EVENT_DECL_P_V(PostRenderEvent);
		ON_EVENT_DECL_P_V(WindowCloseEvent);

		virtual auto init()->void = 0;																//Init underlying window
		virtual auto toggle_fullscreen()->void = 0;													//Toggle fullscreen mode	
		virtual auto set_mouse_pos(double x, double y)->void = 0;
		virtual auto update_vsync()->void = 0;

		virtual auto has_focus()const->bool = 0;

		virtual auto is_vsync()const->bool = 0;
		virtual auto vsync()->bool& = 0;
									
		virtual auto button_state(int button)->int = 0;
		virtual auto mouse_pos()->glm::dvec2 = 0;
		virtual auto width()->uint16_t = 0;															//Get width of window				
		virtual auto height()->uint16_t = 0;														//Get height of window	
		virtual auto title()->std::string = 0;														//Get title of window		
		virtual auto framebuffer_width()->uint16_t = 0;												//Get width of framebuffer
		virtual auto framebuffer_height()->uint16_t = 0;											//Get height of framebuffer
		virtual auto get_time()->double = 0;														//Get time in seconds since initialization of window lib
																									

	};

}