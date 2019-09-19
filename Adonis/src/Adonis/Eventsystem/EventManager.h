#pragma once

#include "Adonis/Core.h"
#include "Event.h"
#include <map>

namespace Adonis {


		class ADONIS_API EventManager {
		public:
			/*
			*	@brief								Add handler with associcated listener id to event subscriptions
			*
			*	@param		event_id				ID of the event, the new handler will be registered to
			*	@param		listener_handler_pair	Pair consisting of listener id and handler function
			*
			*	@return								Boolean representing success or failure while adding handler to list of event's subscriptions
			*/
			static auto subscribe(size_t id, const std::pair<size_t, event_handler>& handler)->bool;

			/*
			*	@brief								Remove handlers, that are associated with listener_id, from event_id's subscription list
			*
			*	@param		event_id				ID of the event, the handler will be removed from
			*	@param		listener_id				ID of the listener that will get its' handlers removed from the event's subscription list
			*
			*/
			static auto unsubscribe(const size_t& event_id, const size_t& listener_id)->void;


			/*
			*	@brief								Add event to static event queue
			*
			*	@param		event					Shared pointer to new event
			*/
			static auto queueEvent(event_ptr event)->void;


			/*
			*	@brief								Execute all handlers that are registered to events currently in the queue
			*
			*/
			static auto processEvents()->void;

			/*
			*	@brief								Get queue size
			*	@return								Current size of event queue
			*
			*/
			static inline auto n_queued_events()->size_t { return s_queue.size(); };

			/*
			*	@brief								Add event to static event queue
			*
			*	@tparam		EventType				Class name of the event to add to the queue
			*	@tparam		Args...					Types of values that will be passed on to the event's constructor
			*/
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