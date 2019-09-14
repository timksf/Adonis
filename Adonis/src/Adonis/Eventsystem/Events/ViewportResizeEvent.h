#pragma once

#include "Adonis/Eventsystem/Event.h"


namespace Adonis {

	class ADONIS_API ViewportResizeEvent : public Adonis::Event {
	public:

		DECLARE_EVENT(ViewportResizeEvent);

		inline ViewportResizeEvent(uint32_t height, uint32_t width) : m_height(height), m_width(width) {};

		inline auto height()->uint32_t { return m_height; };
		inline auto width()->uint32_t { return m_width; };

	private:

		uint32_t m_height;
		uint32_t m_width;

	};

}