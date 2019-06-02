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
			float m_aspectratio{ 16.f / 9.f };
			float m_speed{ 1.0f };
			float m_sensitivity{ 1.0f };

			glm::vec2 m_clip_space{ 10.0f, 100.0f };

			glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_orientation{ 0.f, 0.f, 0.f };
			glm::vec3 m_front{ 0.f, 0.f, 0.f };
			glm::vec3 m_world_up{ 0.f, 1.f, 0.f };
			glm::vec3 m_up{ 0.f, 1.f, 0.f };
			glm::vec3 m_right{ 0.f, 0.f, 0.f };
			glm::vec3 m_down{ 0.f, -1.f, 0.f };

			glm::mat4 m_view{ 1.0f };
			glm::mat4 m_projection{ 1.0f };

			auto update()->void;
		};

	}
}