#pragma once

#include "Adonis/Core.h"
#include "glm/glm.hpp"
#include "Adonis/Eventsystem/EventListener.h"
#include "Adonis/Eventsystem/Events/Events.h"


namespace Adonis {

	namespace rendersystem {

		enum class ADONIS_API MovementDirection {
			Stationary	= 1 << 0,
			Right		= 1 << 1,
			Left		= 1 << 2,
			Up			= 1 << 3,
			Down		= 1 << 4,
			Forward		= 1 << 5,
			Backward	= 1 << 6
		};

		inline constexpr MovementDirection operator|(MovementDirection a, MovementDirection b) {
			return static_cast<MovementDirection>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
		}

		inline constexpr MovementDirection operator&(MovementDirection a, MovementDirection b) {
			return static_cast<MovementDirection>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
		}

		inline constexpr MovementDirection operator^(MovementDirection a, MovementDirection b) {
			return static_cast<MovementDirection>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
		}
		
		inline constexpr MovementDirection& operator|=(MovementDirection& a, MovementDirection b) {
			return a = a | b;
		}

		inline constexpr MovementDirection& operator&=(MovementDirection& a, MovementDirection b) {
			return a = a & b;
		}

		inline constexpr MovementDirection operator~(MovementDirection a) {
			return static_cast<MovementDirection>(~(static_cast<uint32_t>(a)));
		}

		struct ADONIS_API CamInfo {
			glm::vec3* pos{ nullptr };
			glm::vec3* looking_at{ nullptr };
			float* yaw{ nullptr };
			float* pitch{ nullptr };
			float* aspect_ratio{ nullptr };
		};

		class ADONIS_API Camera : EventListener {
		public:

			friend class Scene;

			DECLARE_LISTENER(Camera);

			AD_ON_EVENT_DECL_V(UpdateEvent);
			AD_ON_EVENT_DECL_V(KeyPressed);
			AD_ON_EVENT_DECL_V(KeyReleased);
			AD_ON_EVENT_DECL_V(MouseMovedEvent);
			AD_ON_EVENT_DECL_V(MouseScrolledEvent);

			Camera();

			inline auto enable_zoom()->void { m_enable_zoom = true; };
			inline auto disable_zoom()->void { m_enable_zoom = false; };

			inline auto enable_input()->void { m_enable_input = true; };
			inline auto disable_input()->void { m_enable_input = false; m_first_mouse = true; };

			auto reset_movement()->void;

			auto view()const->glm::mat4;
			auto projection()const->glm::mat4;

		private:
			MovementDirection m_movement_status{ MovementDirection::Stationary };

			float m_fov;
			float m_aspectratio{ 16.f / 9.f };
			float m_velocity{ 5.0f };
			float m_sensitivity{ 0.1f };
			float m_yaw{ 0.0f };
			float m_pitch{ 0.0f };

			double m_lastx{ 0.0f };
			double m_lasty{ 0.0f };

			bool m_constrain_pitch{ true };
			bool m_enable_zoom{ false };
			bool m_enable_input{ false };
			bool m_first_mouse{ false };

			glm::vec2 m_clip_space{ 0.1f, 100.0f };

			glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_front{ 0.f, 0.f, 0.f };
			glm::vec3 m_world_up{ 0.f, 1.f, 0.f };
			glm::vec3 m_up{ 0.f, 1.f, 0.f };
			glm::vec3 m_right{ 0.f, 0.f, 0.f };

			auto update()->void;
		};

	}
}