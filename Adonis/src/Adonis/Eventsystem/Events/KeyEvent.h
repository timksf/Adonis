#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Event.h"

namespace Adonis {


		class ADONIS_API KeyEvent : public Event {
		public:
			DECLARE_EVENT(KeyEvent);

			KeyEvent(int key_code, int mods) : m_code(key_code), m_mods(mods) {};
					
			virtual inline auto code()const->int { return m_code; };
			virtual inline auto mods()const->int { return m_mods; };

		private:
			int m_code{ -1 }; //unknown key as standard
			int m_mods{ -1 }; //unknown key as standard
		};


		class ADONIS_API KeyPressed : public KeyEvent {
		public:
			DECLARE_EVENT(KeyPressed);
			inline KeyPressed(int key_code, int mods) : KeyEvent(key_code, mods) {};
		};

		class ADONIS_API KeyReleased : public KeyEvent {
		public:
			DECLARE_EVENT(KeyReleased);
			inline KeyReleased(int key_code, int mods) : KeyEvent(key_code, mods) {};
		};


}


