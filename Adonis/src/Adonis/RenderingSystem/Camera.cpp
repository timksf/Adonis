#include "pch.h"
#include "Camera.h"
#include "Adonis/Math/Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glm/ext.hpp"

namespace Adonis {

	namespace rendersystem {

		Camera::Camera(){
			AD_ON_EVENT_BIND(KeyPressed, Camera);
			AD_ON_EVENT_BIND(KeyReleased, Camera);
			AD_ON_EVENT_BIND(MouseMovedEvent, Camera);
			AD_ON_EVENT_BIND(MouseScrolledEvent, Camera);
			AD_ON_EVENT_BIND(UpdateEvent, Camera);
			using namespace math::literals;
			m_fov = 100._degf;
		}

		void Camera::update() {
			
			m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			m_front.y = sin(glm::radians(m_pitch));
			m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			m_front = glm::normalize(m_front);

			m_right = glm::normalize(glm::cross(m_front, m_world_up));
			m_up = glm::normalize(glm::cross(m_right, m_front));

		}

		AD_EVENT_FUNC_DEF_HEAD(UpdateEvent, Camera) {

			float distance = m_velocity * event->deltatime();

			if ((MovementDirection::Forward & m_movement_status) == MovementDirection::Forward) {
				m_pos += m_front * distance;
			}
			if ((MovementDirection::Backward & m_movement_status) == MovementDirection::Backward) {
				m_pos -= m_front * distance;
			}
			if ((MovementDirection::Left & m_movement_status) == MovementDirection::Left) {
				m_pos -= m_right * distance;
			}
			if ((MovementDirection::Right & m_movement_status) == MovementDirection::Right) {
				m_pos += m_right * distance;
			}
			if ((MovementDirection::Down & m_movement_status) == MovementDirection::Down) {
				m_pos -= m_world_up * distance;
			}
			if ((MovementDirection::Up & m_movement_status) == MovementDirection::Up) {
				m_pos += m_world_up * distance;
			}

			this->update();

		}

		AD_EVENT_FUNC_DEF_HEAD(KeyPressed, Camera) {
			if (m_enable_input) {
				switch (event->code()) {
				case ADONIS_KEY_W:
					m_movement_status |= MovementDirection::Forward;
					break;
				case ADONIS_KEY_S:
					m_movement_status |= MovementDirection::Backward;
					break;
				case ADONIS_KEY_A:
					m_movement_status |= MovementDirection::Left;
					break;
				case ADONIS_KEY_D:
					m_movement_status |= MovementDirection::Right;
					break;
				case ADONIS_KEY_C:
					m_movement_status |= MovementDirection::Down;
					break;
				case ADONIS_KEY_SPACE:
					m_movement_status |= MovementDirection::Up;
					break;
				}
			}
		}

		AD_EVENT_FUNC_DEF_HEAD(KeyReleased, Camera) {
			if (m_enable_input) {
				switch (event->code()) {
				case ADONIS_KEY_W:
					m_movement_status &= ~MovementDirection::Forward;
					break;
				case ADONIS_KEY_S:
					m_movement_status &= ~MovementDirection::Backward;
					break;
				case ADONIS_KEY_A:
					m_movement_status &= ~MovementDirection::Left;
					break;
				case ADONIS_KEY_D:
					m_movement_status &= ~MovementDirection::Right;
					break;
				case ADONIS_KEY_C:
					m_movement_status &= ~MovementDirection::Down;
					break;
				case ADONIS_KEY_SPACE:
					m_movement_status &= ~MovementDirection::Up;
					break;
				}
			}
		}

		AD_EVENT_FUNC_DEF_HEAD(MouseMovedEvent, Camera) {
			if (m_enable_input) {

				static double last_x = 0;
				static double last_y = 0;

				if (m_first_mouse) {
					last_x = event->xpos();
					last_y = event->ypos();
					m_first_mouse = false;
				}

				double xoff = event->xpos() - last_x;
				double yoff = event->ypos() - last_y;

				last_x = event->xpos();
				last_y = event->ypos();

				m_yaw += xoff * m_sensitivity;
				m_pitch -= yoff * m_sensitivity;

				if (m_constrain_pitch) {
					if (m_pitch > 89.0f) {
						m_pitch = 89.0f;
					}
					else if (m_pitch < -89.0f) {
						m_pitch = -89.0f;
					}
				}

			}
		}

		AD_EVENT_FUNC_DEF_HEAD(MouseScrolledEvent, Camera) {
			if (m_enable_input) {
				if (m_enable_zoom) {
					m_fov -= glm::radians(event->yoffset());
				}
			}
		}

		glm::mat4 Camera::view()const {
			return glm::lookAt(m_pos, m_pos + m_front, m_up);
		}

		glm::mat4 Camera::projection()const {
			return glm::perspective(m_fov, m_aspectratio, m_clip_space.x, m_clip_space.y);
		}

	}
}