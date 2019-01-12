#pragma once

#include "Adonis/Eventsystem/Event.h"


namespace Adonis {
	namespace event {

		class ADONIS_API WindowResizeEvent : public Event {
		public:
			DECLARE_EVENT(WindowResizeEvent);
			inline WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {};

			inline auto width()const->unsigned int { return m_width; };
			inline auto height()const->unsigned int { return m_height; };
		
		private:
			unsigned int m_width;
			unsigned int m_height;
		};

		class ADONIS_API WindowCloseEvent : public Event {
		public:
			DECLARE_EVENT(WindowCloseEvent);
			inline WindowCloseEvent() {};
		};

	}
}
