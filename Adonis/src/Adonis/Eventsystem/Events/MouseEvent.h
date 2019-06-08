#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"

namespace Adonis {

		class MouseButtonEvent : public Event {
		public:
			DECLARE_EVENT(MouseButtonEvent);

			inline MouseButtonEvent(int button, int mods) : m_button(button), m_mods(mods) {};

			inline virtual auto button()const->int { return m_button; };

		private:
			int m_button{ -1 };
			int m_mods{ -1 };
		};

		class MouseButtonPressed : public MouseButtonEvent {
		public:
			DECLARE_EVENT(MouseButtonPressed);
			inline MouseButtonPressed(int button, int mods) : MouseButtonEvent(button, mods) {};
		};

		class MouseButtonReleased : public MouseButtonEvent {
		public:
			DECLARE_EVENT(MouseButtonReleased);
			inline MouseButtonReleased(int button, int mods) : MouseButtonEvent(button, mods) {};
		};

		class MouseMovedEvent : public Event {
		public:
			DECLARE_EVENT(MouseMovedEvent);
			inline MouseMovedEvent(double x, double y){
				m_xpos = x;
				m_ypos = y;
			};

			inline auto xpos()const->double { return m_xpos; };
			inline auto ypos()const->double { return m_ypos; };

		private:
			double m_xpos{ 0 };
			double m_ypos{ 0 };
		};

		class MouseScrolledEvent : public Event {
		public:
			DECLARE_EVENT(MouseScrolledEvent);
			inline MouseScrolledEvent(double xoff, double yoff): m_xoff(xoff), m_yoff(yoff) {};
			
			inline auto xoffset()const->double { return m_xoff; };
			inline auto yoffset()const->double { return m_yoff; };

		private:
			double m_xoff;
			double m_yoff;
		};

}
