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

		enum class ADONIS_API ExitStatus {

			ADONIS_FINE = 0,
			ADONIS_ERROR

		};

		class AppStartEvent : public Event {
		public:
			DECLARE_EVENT(AppStartEvent);
		private:
		};

		class AppStopEvent : public Event {
		public:
			DECLARE_EVENT(AppStopEvent);

			AppStopEvent() = delete;

			AppStopEvent(ExitStatus s) : m_status(s) {};

			inline auto exit_status()->ExitStatus { return m_status; };

		private:
			ExitStatus m_status;
		};

}
