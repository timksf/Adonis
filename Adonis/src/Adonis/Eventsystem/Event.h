#pragma once

#include "Adonis/Core.h"
#include "Adonis/Log.h"

namespace Adonis {

	namespace event {

		class Event {
		public:
			using evID = size_t;
			virtual auto id()const->evID = 0;
			virtual ~Event() {};
		};

		#define	DECLARE_EVENT(type) \
			static Event::evID address(){\
				return reinterpret_cast<Event::evID>(&address);\
			} \
			Event::evID id()const override{\
				return address();\
			}

		class TestEvent: public Event {
		public:
			DECLARE_EVENT(TestEvent);
			inline auto test()const->void { AD_CORE_INFO("Test event"); };
		};

		using event_ptr = std::shared_ptr<Event>;
		template<typename EventType>
		using event_ptr_t = std::shared_ptr<EventType>;
		using event_handler = std::function<void(event_ptr&)>;
		template<typename EventType>
		using event_handler_t = std::function<void(event_ptr_t<EventType>&)>;

	}
}