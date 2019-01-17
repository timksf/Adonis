#pragma once

#include "pch.h"
#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"

namespace Adonis {


		class PreRenderEvent : public Event {
		public:
			DECLARE_EVENT(PreRenderEvent);

		};

		class RenderEvent : public Event {
		public:
			DECLARE_EVENT(RenderEvent);

			RenderEvent(double deltatime) : m_deltatime(deltatime) {};

			inline auto deltatime()const->double { return m_deltatime; };

		private:
			double m_deltatime;
		};

		class PostRenderEvent : public Event {
		public:
			DECLARE_EVENT(PostRenderEvent);

		};

}