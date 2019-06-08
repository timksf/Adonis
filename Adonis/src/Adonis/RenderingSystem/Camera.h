#pragma once

#include "Adonis/Core.h"
#include "glm/glm.hpp"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"


namespace Adonis {

	namespace rendersystem {

		enum class MovementDirection {
			Stationary = 0,
			Right,
			Left,
			Up,
			Down,
			Forward,
			Backward
		};


		class Camera : EventListener {
		public:

			DECLARE_LISTENER(Camera);

			AD_ON_EVENT_DECL_V(UpdateEvent);
			AD_ON_EVENT_DECL_V(KeyPressed);
			AD_ON_EVENT_DECL_V(KeyReleased);
			AD_ON_EVENT_DECL_V(MouseMovedEvent);
			AD_ON_EVENT_DECL_V(MouseScrolledEvent);

			Camera();

			inline auto aspectratio()->float& { return m_aspectratio; };
			inline auto toggle_zoom()->void { m_enable_zoom = !m_enable_zoom; };

			auto view()const->glm::mat4;
			auto projection()const->glm::mat4;

		private:
			MovementDirection m_movement_status{ MovementDirection::Stationary };

			float m_fov;
			float m_aspectratio{ 16.f / 9.f };
			float m_velocity{ 1.0f };
			float m_sensitivity{ 1.0f };
			float m_yaw{ 0.0f };
			float m_pitch{ 0.0f };

			bool m_constrain_pitch = false;
			bool m_enable_zoom = false;

			glm::vec2 m_clip_space{ 10.0f, 100.0f };

			glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_front{ 0.f, 0.f, 0.f };
			glm::vec3 m_world_up{ 0.f, 1.f, 0.f };
			glm::vec3 m_up{ 0.f, 1.f, 0.f };
			glm::vec3 m_right{ 0.f, 0.f, 0.f };
			glm::vec3 m_down{ 0.f, -1.f, 0.f };


			auto update()->void;
		};

	}
}