#pragma once

#include "Adonis/Core.h"
#include "Mesh.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {

	namespace rendersystem {

		class Model {
		public:

			Model() = delete;

			Model(std::unique_ptr<Mesh>&& mesh);

			auto mesh_specs()const->MeshSpecification;
			auto primitive_count()const->uint32_t;
			auto vao()const->std::unique_ptr<render::VertexArray>;

		private:
			std::unique_ptr<Mesh> m_mesh;

			glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

			glm::mat4 m_matrix{ 1.0f };
		};

	}
}