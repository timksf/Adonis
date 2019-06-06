#pragma once

#include "Adonis/Core.h"
#include "Mesh.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {

	namespace rendersystem {

		class Model: EventListener{
		public:

			DECLARE_LISTENER(Model);

			ON_EVENT_DECL_V(UpdateEvent);

			Model() = delete;

			Model(std::unique_ptr<Mesh>&& mesh);

			auto mesh_specs()const->MeshSpecification;
			auto primitive_count()const->uint32_t;
			auto create_vao()const->std::unique_ptr<render::VertexArray>;

			auto is_indexed()->bool;
			auto vbo_id()->uint32_t;
			auto ibo_id()->uint32_t;

			inline auto matrix()->glm::mat4& { return m_matrix; };

		private:
			std::unique_ptr<Mesh> m_mesh;

			glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

			glm::mat4 m_matrix{ 1.0f };
		};

	}
}