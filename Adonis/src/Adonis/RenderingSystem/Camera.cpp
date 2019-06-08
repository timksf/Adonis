#include "pch.h"
#include "Camera.h"
#include "Adonis/Math/Math.h"

namespace Adonis {

	namespace rendersystem {

		Camera::Camera(){
			using namespace math::literals;
			m_fov = 90._degf;
		}

		glm::mat4& Camera::view() {
			return m_view;
		}

		glm::mat4 Camera::view() const {
			return m_view;
		}

		glm::mat4& Camera::projection() {
			return m_projection;
		}

		glm::mat4 Camera::projection() const {
			return m_projection;
		}

		void Camera::update() {
			// orientation.x = roll, orientation.y = pitch, orientation.z = yaw
			m_front.x = cos(glm::radians(m_orientation.z)) * cos(glm::radians(m_orientation.y));
			m_front.y = sin(glm::radians(m_orientation.y));
			m_front.z = sin(glm::radians(m_orientation.z)) * cos(glm::radians(m_orientation.y));
			m_front = glm::normalize(m_front);

			m_right = glm::normalize(glm::cross(m_front, m_world_up));
			m_up = glm::normalize(glm::cross(m_right, m_front));
		}

		AD_EVENT_FUNC_DEF_HEAD(UpdateEvent, Camera) {

			float distance = m_velocity * event->deltatime();

			switch (m_movement_status) {
			case MovementDirection::Forward:
				m_pos += m_front * distance;
				break;
			case MovementDirection::Backward:
				m_pos -= m_front * distance;
				break;
			case MovementDirection::Left:
				m_pos -= m_right * distance;
				break;
			case MovementDirection::Right:
				m_pos += m_right * distance;
				break;
			case MovementDirection::Down:
				m_pos -= m_up * distance;
				break;
			case MovementDirection::Up:
				m_pos += m_up * distance;
				break;
			case MovementDirection::Stationary: //No need to move
			default: break;
			}

		}

		AD_EVENT_FUNC_DEF_HEAD(KeyPressed, Camera) {
			switch (event->code()) {
			case ADONIS_KEY_W:
				m_movement_status = MovementDirection::Forward;
				break;
			case ADONIS_KEY_S:
				m_movement_status = MovementDirection::Backward;
				break;
			case ADONIS_KEY_A:
				m_movement_status = MovementDirection::Left;
				break;
			case ADONIS_KEY_D:
				m_movement_status = MovementDirection::Right;
				break;
			case ADONIS_KEY_C:
				m_movement_status = MovementDirection::Down;
				break;
			case ADONIS_KEY_SPACE:
				m_movement_status = MovementDirection::Up;
				break;
			}
		}

		AD_EVENT_FUNC_DEF_HEAD(KeyReleased, Camera) {
			switch (event->code()) {
			default: 
				m_movement_status = MovementDirection::Stationary;
				break;
			}
		}

	}
}