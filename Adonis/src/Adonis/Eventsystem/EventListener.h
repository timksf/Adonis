#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"
#include "Adonis/Eventsystem/EventManager.h"
#include "Adonis/Log.h"

namespace Adonis {
		//class that keeps track of subscribed events w/ automatic unsubscribing
		class ADONIS_API EventListener {
		public:

			virtual auto id()const->size_t = 0;

			virtual ~EventListener() {
				for (auto& subscription : m_subscriptions) {
					EventManager::unsubscribe(subscription.first, subscription.second);
				}
			}


			template<typename EventType>
			inline auto on_event(const event_handler_t<EventType>& handler)->void{
				auto casted_handler = ([&, handler](event_ptr event) { //pass event_ptr by reference and handler by value
					auto&& casted_event = std::dynamic_pointer_cast<EventType>(event);
					if (casted_event != nullptr) {
						handler(casted_event);
					}
					else {
						AD_CORE_ERROR("Passed pointer could not be resolved to an event type");
					}
				});

				init(EventType::address(), casted_handler);
			}

		protected:

			inline auto init(const Event::evID& id, event_handler handler)->void{
				size_t offset = EventManager::subscribe(id, std::make_pair(this->id(), handler));
				m_subscriptions.push_back(std::make_pair(id, offset));
			}

		private:
			std::vector<std::pair<Event::evID, size_t>> m_subscriptions;
		};

		#define DECLARE_LISTENER(type)\
		static size_t address(){ return reinterpret_cast<size_t>(&address); };\
		size_t id()const override{ return address(); };

}


