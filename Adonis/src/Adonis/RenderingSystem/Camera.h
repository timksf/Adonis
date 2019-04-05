#pragma once

#include "Adonis/Core.h"
#include "glm/glm.hpp"


namespace Adonis {

	namespace rendersystem {

		class Camera {
		public:

			Camera();

			auto view()->glm::mat4&;
			auto view()const->glm::mat4;
			auto projection()->glm::mat4&;
			auto projection()const->glm::mat4;

		private:
			float m_fov;
			float m_aspectratio;
			float m_speed;
			float m_sensitivity;

			glm::vec2 m_clip_space;

			glm::vec3 m_pos;
			glm::vec3 m_orientation;
			glm::vec3 m_front;
			glm::vec3 m_world_up;
			glm::vec3 m_up;
			glm::vec3 m_right;
			glm::vec3 m_down;

			glm::mat4 m_view;
			glm::mat4 m_projection;

			auto update()->void;
		};

	}
}