#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include <string>
#include <cstdint>

namespace Adonis {

	enum class WindowMode {
		Windowed,
		Borderless,
		Fullscreen
	};

	class ADONIS_API IWindow: public event::EventListener{
	public:

		static std::unique_ptr<IWindow> create(const uint16_t& width = 640, const uint16_t& height = 480, const std::string& title = "Window", WindowMode mode = WindowMode::Windowed, const bool& vsnyc = true);

		virtual ~IWindow() {};

		virtual auto on_update(event::event_ptr_t<event::UpdateEvent>& ev)->void = 0;
		virtual auto init()->void = 0;
		virtual auto toggle_fullscreen()->void = 0;

		virtual auto width()->uint16_t = 0;
		virtual auto height()->uint16_t = 0;
		virtual auto title()->std::string = 0;

	};

}