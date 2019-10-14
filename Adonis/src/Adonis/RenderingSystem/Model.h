#pragma once

#include "Adonis/Core.h"
#include "Mesh.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {

	namespace rendersystem {

		class ADONIS_API Model: public EventListener{
		public:

			DECLARE_LISTENER(Model);

			AD_ON_EVENT_DECL_V(UpdateEvent);

			Model() = delete;

			Model(std::unique_ptr<Mesh>&& mesh, const char* name);

			auto mesh_specs()const->MeshSpecification;
			auto primitive_count()const->uint32_t;
			auto create_vao()const->std::unique_ptr<render::VertexArray>;

			auto is_indexed()->bool;
			auto vbo_id()->uint32_t;
			auto ibo_id()->uint32_t;

			auto initialized()->bool;

			inline auto matrix()->glm::mat4& { return m_matrix; };
			inline auto name()->std::string { return m_name; };
			inline auto set_name(const char* name)->void { m_name = std::string(name); };

			inline auto pos()->glm::vec3& { m_update_pos = true;  return m_pos; };
			inline auto rotation()->glm::vec3& { m_update_rotation = true; return m_rotation; };
			inline auto scale()->glm::vec3& { m_update_scale = true; return m_scale; };


		private:

			auto update_pos()->void;
			auto update_rotation()->void;
			auto update_scale()->void;
			auto update()->void;

			std::string m_name;
			std::unique_ptr<Mesh> m_mesh;

			glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

			glm::mat4 m_matrix{ 1.0f };

			bool m_update_pos;
			bool m_update_scale;
			bool m_update_rotation;
		};

	}
}