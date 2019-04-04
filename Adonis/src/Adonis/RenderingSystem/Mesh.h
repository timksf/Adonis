#pragma once

#include <unordered_map>
#include "Adonis/Core.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {

	namespace rendersystem {

		class MeshSpecification {
		public:

			MeshSpecification() = delete;

			MeshSpecification(std::shared_ptr<render::VertexArrayDesc> desc, render::DrawMode mode = render::DrawMode::Triangles, render::DrawMethod method = render::DrawMethod::Classic);

			auto operator==(const MeshSpecification& rhs)const->bool;

			inline auto mode()const->render::DrawMode { return m_draw_mode; };

			inline auto method()const->render::DrawMethod { return m_draw_method; };

			inline auto buffer_desc()const->std::shared_ptr<render::VertexArrayDesc> { return m_buffer_desc; };

		private:
			render::DrawMode m_draw_mode;
			render::DrawMethod m_draw_method;
			std::shared_ptr<render::VertexArrayDesc> m_buffer_desc;
		};

		template<typename T> class Hasher;

		template<>
		class Hasher<MeshSpecification> {
		public:

			size_t operator()(const MeshSpecification& mg) const {
				auto hash1 = std::hash<uint32_t>()(static_cast<uint32_t>(mg.mode()));
				auto hash2 = std::hash<uint32_t>()(static_cast<uint32_t>(mg.method()));

				return hash1 ^ (hash2 << 1);
			}

		};


		class Mesh {
		public:

			Mesh();

			//Mesh(void* vertices, uint32_t nvertices, void* indices = nullptr, uint32_t nindices = 0, render::DrawMode mode = render::DrawMode::Triangles);

			auto specs()const->MeshSpecification;
			auto prim_count()const->uint32_t;

		private:
			uint32_t m_ibo;
			uint32_t m_vbo;
			uint32_t m_vao;
			uint32_t m_n_vertices;
			uint32_t m_n_indices;
			uint32_t m_prim_count;

			std::unique_ptr<render::VertexArrayDesc> m_buffer_desc;

			MeshSpecification m_specs;

		};


	}
}