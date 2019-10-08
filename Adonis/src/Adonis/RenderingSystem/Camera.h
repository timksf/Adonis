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
			inline auto toggle_zoom()->void { m_enable_zoom = !m_enable_zoom; };


			inline auto enable_input()->void { m_enable_input = true; };
			inline auto disable_input()->void { m_enable_input = false; m_first_mouse = true; };
			inline auto toggle_input()->void { m_enable_input = !m_enable_input; if (!m_enable_input) m_first_mouse = true; };

			inline auto toggle_pitch_constraint()->void { m_constrain_pitch = !m_constrain_pitch; };

			auto reset_movement()->void;

			auto view()const->glm::mat4;
			auto projection()const->glm::mat4;

			inline auto fov()				->float&				{ return m_fov; };
			inline auto aspect_ratio()		->float&				{ return m_aspectratio; };
			inline auto vel()				->float&				{ return m_velocity; };
			inline auto sens()				->float&				{ return m_sensitivity; };
			inline auto yaw()				->float&				{ return m_yaw; };
			inline auto pitch()				->float&				{ return m_pitch; };
			inline auto pitch_constrained()	->bool&					{ return m_constrain_pitch; };
			inline auto zoom_enabled()		->bool&					{ return m_enable_zoom; };
			inline auto input_enabled()		->bool&					{ return m_enable_input; };
			inline auto pos()				->glm::vec3&			{ return m_pos; };
			inline auto viewing_dir()		->glm::vec3		const	{ return m_front; };
			inline auto clip_space()		->glm::vec2& const		{ return m_clip_space; };


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