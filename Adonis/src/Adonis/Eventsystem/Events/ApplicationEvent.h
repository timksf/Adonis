#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"
#include "Adonis/Eventsystem/EventListener.h"

namespace Adonis {

		class UpdateEvent: public Event {
		public:
			DECLARE_EVENT(UpdateEvent);	

			UpdateEvent() = delete;
			
			UpdateEvent(double deltatime) : m_deltatime(deltatime) {};

			inline auto deltatime()const->double { return m_deltatime; };

		private:
			double m_deltatime;
		};

		class AppStartEvent : public Event {
		public:
			DECLARE_EVENT(AppStartEvent);
		};

		class AppStopEvent : public Event {
		public:
			DECLARE_EVENT(AppStopEvent);
		};

}
