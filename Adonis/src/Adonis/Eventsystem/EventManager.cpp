#include "pch.h"
#include "EventManager.h"
#include "Adonis/Log.h"

namespace Adonis {

		std::vector<event_ptr> EventManager::s_queue = std::vector<event_ptr>();
		std::map<Event::evID, std::vector<std::pair<size_t, event_handler>>> EventManager::s_subscriptions = std::map<Event::evID, std::vector<std::pair<size_t, event_handler>>>();

		
		bool EventManager::subscribe(size_t event_id, const std::pair<size_t, event_handler>& listener_handler_pair) {
			//Determine if event of type event_id already has a subscriber list
			auto where = s_subscriptions.find(event_id);

			//If event of type event_id does not have a subscriber list yet, initialize new list with size of 2
			bool has_subs = true;
			if (where == s_subscriptions.end()) {
				s_subscriptions[event_id] = std::vector<std::pair<size_t, event_handler>>();
				s_subscriptions[event_id].reserve(2);
				has_subs = false;
			}
		
			auto& registered_handlers = s_subscriptions[event_id];

			//If event of type event_id already has a list of subscribers, check if listener is already registered in this list
			//If the case, just return false, as one class cannot have two different functions subscribed to the same event
			if (has_subs) {
				for (auto& entry : registered_handlers) {
					if (entry.first == listener_handler_pair.first) {
						AD_CORE_ERROR("Trying to register function to event from instance that is already subscribed to this event");
						return false;
					}
				}
			}

			//Add event handler to the subscriber list of the event
			registered_handlers.push_back(listener_handler_pair);
			return true;
		}

		void EventManager::unsubscribe(const size_t& ev_id, const size_t& listener_id) {
			auto where = s_subscriptions.find(ev_id);

			if (where != s_subscriptions.end()) {
				auto& registered_handlers = s_subscriptions[ev_id];
				for (auto it = registered_handlers.begin(); it != registered_handlers.end();) {
					auto& entry = *it;
					if (entry.first == listener_id) {
						it = registered_handlers.erase(it);
					}
					else {
						it++;
					}
				}
			}
		}

		void EventManager::queueEvent(event_ptr event) {
			s_queue.push_back(event);
		}

		void EventManager::processEvents() {
			//AD_CORE_WARN("Number of events in queue: {0}", s_queue.size());
			while (s_queue.size() > 0) {
				auto&& entry = s_subscriptions.find(s_queue[0]->id());
				if (entry != s_subscriptions.end()) {
					auto& handlers = entry->second;
					for (auto& handler : handlers) {
						handler.second(s_queue[0]);
					}
				}
				s_queue.erase(s_queue.begin());
			}
		}
}