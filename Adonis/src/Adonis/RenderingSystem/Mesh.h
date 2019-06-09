#pragma once

#include <unordered_map>
#include "Adonis/Core.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {

	namespace rendersystem {

		class ADONIS_API MeshSpecification {
		public:

			MeshSpecification() = delete;

			MeshSpecification(std::shared_ptr<render::VertexArrayDesc> desc, render::DrawMode mode = render::DrawMode::Triangles, render::DrawMethod method = render::DrawMethod::Classic);

			auto operator==(const MeshSpecification& rhs)const->bool;

			inline auto set_buffer_desc(std::shared_ptr<render::VertexArrayDesc> buffer_desc)->void { m_buffer_desc = buffer_desc; };

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


		class ADONIS_API Mesh {
		public:

			Mesh() = delete;

			Mesh(render::DrawMode, render::DrawMethod = render::DrawMethod::Classic);

			Mesh(std::shared_ptr<render::VertexArrayDesc> buffer_desc, render::DrawMode = render::DrawMode::Triangles, render::DrawMethod = render::DrawMethod::Classic);

			Mesh(void* vertices, uint32_t nvertices, std::shared_ptr<render::VertexArrayDesc> buffer_desc, void* indices = nullptr, uint32_t nindices = 0, render::DrawMode mode = render::DrawMode::Triangles);

			auto specs()const->MeshSpecification;
			auto prim_count()const->uint32_t;

			auto set_data(void* vertices, uint32_t n_vertices, void* indices, uint32_t n_indices)->void;

			auto set_buffer_desc(std::shared_ptr<render::VertexArrayDesc> buffer_desc)->void;
			 
			inline auto vbo_id()->uint32_t { return m_vbo->id(); };
			inline auto ibo_id()->uint32_t { return m_ibo->id(); };
			inline auto initialized()->bool { return m_buffers_initialized && m_specs_initialized; };

		private:

			auto init_buffers(void* vertices, uint32_t n_vertices, void* indices = nullptr, uint32_t n_indices = 0)->void;
			auto init_specs(std::shared_ptr<render::VertexArrayDesc> buffer_desc, render::DrawMode draw_mode, render::DrawMethod method)->void;

			std::unique_ptr<render::IndexBuffer> m_ibo{ nullptr };
			std::unique_ptr<render::VertexBuffer> m_vbo{ nullptr };
			uint32_t m_n_vertices{ 0 };
			uint32_t m_n_indices{ 0 };
			uint32_t m_prim_count{ 0 };

			bool m_buffers_initialized{ false };
			bool m_specs_initialized{ false };

			MeshSpecification m_specs{ nullptr, render::DrawMode::Triangles, render::DrawMethod::Classic };

		};


	}
}