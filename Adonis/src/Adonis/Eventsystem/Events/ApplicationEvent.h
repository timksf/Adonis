#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"
#include "Adonis/Eventsystem/EventListener.h"

namespace Adonis {

		class UpdateEvent: public Event {
		public:
			DECLARE_EVENT(UpdateEvent);
		};

}
