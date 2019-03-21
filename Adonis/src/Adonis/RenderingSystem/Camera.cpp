#include "pch.h"
#include "Camera.h"
#include "Adonis/Math/Math.h"


namespace Adonis::rendersystem {

	Camera::Camera():
		m_aspectratio(16.f/9.f),
		m_speed(1.f),
		m_sensitivity(1.f),
		m_pos({0.f, 0.f, 0.f}),
		m_orientation({ 0.f, 0.f, 0.f }),
		m_front({ 0.f, 0.f, 0.f }),
		m_right({ 0.f, 0.f, 0.f }),
		m_world_up({0.f, 1.f, 0.f}),
		m_up({0.f, 1.f, 0.f}),
		m_down({ 0.f, -1.f, 0.f }),
		m_view(1.f),
		m_projection(1.f)
	{
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

}