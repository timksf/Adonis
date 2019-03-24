#pragma once

#include "Adonis/Core.h"
#include "Mesh.h"

namespace Adonis::rendersystem {

	class Model {
	public:

		Model() = delete;

		Model(std::unique_ptr<Mesh>&& mesh);

		auto mesh_specs()const->MeshSpecification;
		auto primitive_count()const->uint32_t; 

	private:
		std::unique_ptr<Mesh> m_mesh;

		glm::vec3 m_pos;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		glm::mat4 m_matrix;
	};

}