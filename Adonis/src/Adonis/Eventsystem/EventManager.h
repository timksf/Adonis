#pragma once

#include "Adonis/Core.h"
#include "Event.h"
#include <map>

namespace Adonis {

	namespace event {

		//class IEventManager {
		//public:
		//	virtual auto subscribe(size_t id, event_handler handler)->bool = 0;
		//	virtual auto unsubscribe(size_t id, event_handler handler)->bool = 0;
		//	virtual auto queueEvent(event_ptr event)->void = 0;
		//	virtual auto processEvents()->void = 0;
		//};

		class ADONIS_API EventManager {
		public:
			static auto subscribe(size_t id, const event_handler& handler)->bool;
			static auto unsubscribe(size_t id, const event_handler& handler)->bool;
			static auto queueEvent(event_ptr event)->void;
			static auto processEvents()->void;

			template<typename EventType>
			static inline auto queueEvent()->void {
				if (!valid_type<EventType>()) {
					#ifdef ADONIS_DEBUG
						AD_CORE_WARN("Failed to queue event of type: {0}", typeid(EventType).name());
					#endif
					return;
				}else{
					queueEvent(std::make_shared<EventType>());
				}
			}

		private:

			/*
				Check if EventType actually is derived from event class
			*/
			template<typename EventType>
			static inline auto valid_type()->bool {
				return std::is_base_of_v<Event, EventType>;
			}

			static std::vector<event_ptr> s_queue;
			static std::map<Event::evID, std::vector<event_handler>> s_subscriptions;
		};

	}
}