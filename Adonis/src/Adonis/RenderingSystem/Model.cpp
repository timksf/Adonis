#include "pch.h"
#include "Model.h"

namespace Adonis::rendersystem {

	Model::Model(std::unique_ptr<Mesh>&& mesh) :
		m_mesh(std::move(mesh)){

	}

	MeshSpecification Model::mesh_specs()const {
		return m_mesh->specs();
	}

	uint32_t Model::primitive_count()const {
		return m_mesh->prim_count();
	}

	std::unique_ptr<render::VertexArray> Model::create_vao()const {
		return render::VertexArray::create(m_mesh->specs().buffer_desc());
	}

	bool Model::is_indexed() {
		return m_mesh->specs().method() == render::DrawMethod::Indexed;
	}

	uint32_t Model::vbo_id() {
		return m_mesh->vbo_id();
	}
	
	uint32_t Model::ibo_id() {
		return m_mesh->ibo_id();
	}


}