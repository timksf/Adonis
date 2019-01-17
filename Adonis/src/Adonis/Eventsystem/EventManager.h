#pragma once

#include "Adonis/Core.h"
#include "Event.h"
#include <map>

namespace Adonis {


		class ADONIS_API EventManager {
		public:
			static auto subscribe(size_t id, const std::pair<size_t, event_handler>& handler)->size_t;
			static auto unsubscribe(size_t ev_id, const size_t& listener_id)->bool;
			static auto queueEvent(event_ptr event)->void;
			static auto processEvents()->void;

			static inline auto n_queued_events()->size_t { return s_queue.size(); };

			template<typename EventType, typename... Args>
			static inline auto queueEvent(Args&&... args)->void {
				if (!valid_type<EventType>()) {
					#ifdef ADONIS_DEBUG
						AD_CORE_WARN("Failed to queue event of type: {0}", typeid(EventType).name());
					#endif
					return;
				}else{
					queueEvent(std::make_shared<EventType>(std::forward<Args>(args)...));
				}
			}

		private:

			/*
				Check if EventType is derived from event class
			*/
			template<typename EventType>
			static inline auto valid_type()->bool {
				return std::is_base_of_v<Event, EventType>;
			}

			static std::vector<event_ptr> s_queue;
			static std::map<Event::evID, std::vector<std::pair<size_t, event_handler>>> s_subscriptions;
		};

}