#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"
#include "Adonis/Eventsystem/EventManager.h"
#include "Adonis/Log.h"

namespace Adonis {
	namespace event {
		//class that keeps track of subscribed events w/ automatic unsubscribing
		//TODO add support for multiple EventTypes
		class ADONIS_API EventListener {
		public:

			virtual ~EventListener() {
				for (auto& subscription : m_subscriptions) {
					EventManager::unsubscribe(subscription.first, subscription.second);
				}
			}

			template<typename EventType>
			inline auto init(const event_handler_t<EventType>& handler)->void{
				auto casted_handler = ([&, handler](event_ptr event) { //pass event_ptr by reference and handler by value
					auto&& casted_event = std::dynamic_pointer_cast<EventType>(event);
					if (casted_event != nullptr) {
						handler(casted_event);
					}
					else {
						AD_CORE_ERROR("Passed pointer could not be resolved to an event");
					}
				});

				init(EventType::address(), casted_handler);
			}

		protected:

			inline auto init(const Event::evID& id, event_handler handler)->void{
				//Try to sub to event
				if (EventManager::subscribe(id, handler)) {
					m_subscriptions.push_back(std::make_pair(id, handler));
				}
			}

		private:

			std::vector<std::pair<Event::evID, event_handler>> m_subscriptions;
		};

	}
}


