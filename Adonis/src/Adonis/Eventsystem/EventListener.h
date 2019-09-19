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
				/*for (auto& subscription : m_subscriptions) {
					EventManager::unsubscribe(subscription.first, m_id);
				}*/
				for (auto& evid : m_subscriptions) {
					EventManager::unsubscribe(evid, m_id);
				}
			}

			/*
			*	@brief					Generic on_event function
			*	@details				Listener classes deriving from EventListener call this function with an EventType in the template and a function(mostly member functions of the derived class)
			*							to "subscribe" to the Event and get the passed function executed each time an event of EventType is being removed from the event-queue of the EventManager
			*
			*	@tparam		EventType	The type of the event, the passed handler is capable of processing
			*	@param		handler		The function that will handle the event of type EventType
			*/
			template<typename EventType>
			inline auto on_event(const event_handler_t<EventType>& handler)->void{

				//The EventManager stores the handlers in a map as event_handler(s), create casted handler, which is 
				//able to accept a pointer to the base Event class
				auto casted_handler = ([&, handler](event_ptr event) { 
					//In order to be able to pass the event to the handler which was passed to the on_event function,
					//downcast the pointer of the base class to the EventType pointer the handler takes as a param
					auto&& casted_event = std::dynamic_pointer_cast<EventType>(event);

					//If the cast fails, then there's something wrong with the provided EventType and the casted_event will be a nullptr,
					//to prevent a crash, simply avoid executing the handler
					if (casted_event != nullptr) {
						handler(casted_event);
					}
					else {
						//If the pointer cast failed, print an error every time the handler is triggered
						AD_CORE_ERROR("Passed pointer could not be resolved to an event type");
					}
				});

				//Pass the event handler to the EventManager
				init(EventType::address(), casted_handler);
			}

		private:

			/*
			*	@brief					Register new handler for event 
			*
			*	@details				Passes EventID and handler to EventManager, which then handles actual event queuing/handler execution
			*							EventListener keeps track of events it has subscribed to, to be able to unsubscribe later
			*
			*	@param		id			EventID of event being subscribed to
			*	@param		handler		Handler which takes pointer to Event base class (casted handler)
			*
			*/
			inline auto init(const Event::evID& id, event_handler handler)->void{
				//Tell EventManager to add a new handler for event of type id, pass listener's id as well
				EventManager::subscribe(id, std::make_pair(this->id(), handler));
				//Add event id to listener's subscriptions
				m_subscriptions.push_back(id);
				//Initialize listener id, if that hasn't happened already
				if(m_id == 0)
					m_id = this->id();
			}

			std::vector<Event::evID> m_subscriptions;
			size_t m_id{ 0 };
		};

		#define DECLARE_LISTENER(type)\
		size_t id()const override{ return reinterpret_cast<uint32_t>(this); };

}


