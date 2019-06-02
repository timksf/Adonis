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

	std::unique_ptr<render::VertexArray> Model::vao()const {
		return render::VertexArray::create(m_mesh->specs().buffer_desc());
	}



}