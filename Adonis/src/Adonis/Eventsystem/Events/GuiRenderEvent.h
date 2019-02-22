#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"

namespace Adonis {

	class ADONIS_API GuiRenderEvent : public Event {
	public:
		DECLARE_EVENT(GuiRenderEvent);
	};

}
