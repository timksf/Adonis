#include "pch.h"
#include "EventManager.h"
#include "Adonis/Log.h"

namespace Adonis {

		std::vector<event_ptr> EventManager::s_queue = std::vector<event_ptr>();
		std::map<Event::evID, std::vector<std::pair<size_t, event_handler>>> EventManager::s_subscriptions = std::map<Event::evID, std::vector<std::pair<size_t, event_handler>>>();

		size_t EventManager::subscribe(size_t id, const std::pair<size_t, event_handler>& handler) {
			auto where = s_subscriptions.find(id);

			if (where == s_subscriptions.end()) {
				#ifdef ADONIS_DEBUG
					AD_CORE_INFO("{0}, adding i: {1} to subscription map", typeid(EventManager).name(), id);
				#endif
				s_subscriptions[id] = std::vector<std::pair<size_t, event_handler>>();
			}
			{
				//Check if handler function already exists
				auto& registered_handlers = s_subscriptions[id];
				for (auto& entry : registered_handlers) {
					if (entry.first == handler.first) {
						return false;
					}
				}
				#ifdef ADONIS_DEBUG
					AD_CORE_INFO("{0}, registered new handler for event with id: {1}", typeid(EventManager).name(), id);
				#endif		
				registered_handlers.push_back(handler);
				return handler.first;
			}
		}

		bool EventManager::unsubscribe(size_t ev_id, const size_t& listener_id) {
			auto where = s_subscriptions.find(ev_id);

			if (where != s_subscriptions.end()) {
				auto& registered_handlers = s_subscriptions[ev_id];
				for (auto it = registered_handlers.begin(); it != registered_handlers.end();) {
					auto& entry = *it;
					if (entry.first == listener_id) {
						#ifdef ADONIS_DEBUG
							AD_CORE_INFO("Unsubscribing from event {0}", ev_id);
						#endif ADONIS_DEBUG
						it = registered_handlers.erase(it);
					}
					else {
						it++;
					}
				}
			}

			return false;
		}

		void EventManager::queueEvent(event_ptr event) {
			s_queue.push_back(event);
		}

		void EventManager::processEvents() {
			//AD_CORE_INFO("New iter");
			while (s_queue.size() > 0) {
				//AD_CORE_INFO("Processing event: {0}", s_queue[0]->name());
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