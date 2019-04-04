#include "pch.h"
#include "Mesh.h"

namespace Adonis {

	namespace rendersystem {

		MeshSpecification::MeshSpecification(std::shared_ptr<render::VertexArrayDesc> desc, render::DrawMode mode, render::DrawMethod method) :
			m_draw_mode(mode),
			m_draw_method(method),
			m_buffer_desc(desc)
		{
		};


		Mesh::Mesh() :
			m_n_indices(0),
			m_n_vertices(0),
			m_vbo(0),
			m_ibo(0),
			m_vao(0),
			m_prim_count(0),
			m_specs(MeshSpecification(nullptr, render::DrawMode::Triangles))
		{

		};

		/*Mesh::Mesh(void* vertices, uint32_t nvertices, void* indices, uint32_t nindices, render::DrawMode draw_mode):
			m_n_indices(nindices),
			m_n_vertices(nvertices)
		{
			render::DrawMethod draw_method;
			if (m_n_indices == 0) {
				draw_method = render::DrawMethod::Classic;
			}
			else {
				draw_method = render::DrawMethod::Indexed;
			}

			m_specs = MeshSpecification(draw_mode, draw_method);
		}*/

		bool MeshSpecification::operator==(const MeshSpecification& rhs) const {
			return (m_draw_mode == rhs.m_draw_mode) && (m_draw_method == rhs.m_draw_method) && (m_buffer_desc == rhs.m_buffer_desc);
		}

		MeshSpecification Mesh::specs()const {
			return m_specs;
		}

		uint32_t Mesh::prim_count()const {
			return m_prim_count;
		}

	}
}