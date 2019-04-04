#include "pch.h"
#include "Model.h"

namespace Adonis::rendersystem {

	Model::Model(std::unique_ptr<Mesh>&& mesh) :
		m_mesh(std::move(mesh)),
		m_pos({0.f, 0.f, 0.f}),
		m_rotation({0.f, 0.f, 0.f}),
		m_scale({1.f, 1.f, 1.f}),
		m_matrix(1.f)
	{

	};

	MeshSpecification Model::mesh_specs()const {
		return m_mesh->specs();
	}

	uint32_t Model::primitive_count()const {
		return m_mesh->prim_count();
	}



}