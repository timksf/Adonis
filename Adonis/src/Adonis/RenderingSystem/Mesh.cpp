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


		Mesh::Mesh(std::shared_ptr<render::VertexArrayDesc> buffer_desc, render::DrawMode draw_mode, render::DrawMethod draw_method) {
			m_specs = MeshSpecification(buffer_desc, draw_mode, draw_method);
		}


		Mesh::Mesh(void* vertices, uint32_t nvertices, std::shared_ptr<render::VertexArrayDesc> buffer_desc, void* indices, uint32_t nindices, render::DrawMode draw_mode):
			m_n_indices(nindices),
			m_n_vertices(nvertices)
		{
			this->init_buffers(vertices, nvertices, indices, nindices);

			m_specs = MeshSpecification(buffer_desc, draw_mode, (nindices == 0 || indices == nullptr) ? render::DrawMethod::Classic : render::DrawMethod::Indexed);
		}

		bool MeshSpecification::operator==(const MeshSpecification& rhs) const {
			return (m_draw_mode == rhs.m_draw_mode) && (m_draw_method == rhs.m_draw_method) && (m_buffer_desc == rhs.m_buffer_desc);
		}

		MeshSpecification Mesh::specs()const {
			return m_specs;
		}

		uint32_t Mesh::prim_count()const {
			return m_prim_count;
		}

		void Mesh::set_data(void* vertices, uint32_t n_vertices, void* indices, uint32_t n_indices) {

		}

		void Mesh::init_buffers(void* vertices, uint32_t n_vertices, void* indices, uint32_t n_indices) {
			if (m_n_indices == 0 || indices == nullptr) {
				m_prim_count = static_cast<uint32_t>(std::floor(m_n_vertices / render::AD_LOOKUP_CORE(drawmode_divisor, m_specs.mode())));
			}
			else {
				//TODO add functionality to be able to influence buffer flags upon mesh creation
				m_ibo = render::IndexBuffer::create(m_n_indices, indices, render::BufferBit::DYNAMIC_STORAGE);
				m_prim_count = static_cast<uint32_t>(std::floor(m_n_indices / render::AD_LOOKUP_CORE(drawmode_divisor, m_specs.mode())));
			}

			//TODO add functionality to be able to influence buffer flags upon mesh creation
			m_vbo = render::VertexBuffer::create(m_n_vertices, vertices, render::BufferBit::DYNAMIC_STORAGE | render::BufferBit::MAP_READ);

			m_buffers_initialized = true;
		}

	}
}