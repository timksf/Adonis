#include "pch.h"
#include "Model.h"

namespace Adonis::rendersystem {

	Model::Model(std::unique_ptr<Mesh>&& mesh, const char* name): m_name(name){
		AD_CORE_ASSERT(mesh->initialized(), "Model: Mesh not initialized properly");
		AD_ON_EVENT_BIND(UpdateEvent, Model);
		m_mesh = std::move(mesh);
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

	void Model::on_UpdateEvent(const event_ptr_t<UpdateEvent>& event) {
		update();
	}

	bool Model::initialized() {
		return !m_name.empty();
	}

	void Model::update() {
		if(m_update_pos)			update_pos();
		if(m_update_scale)			update_scale();
		if(m_update_rotation)		update_rotation();
	}

	void Model::update_pos() {
		m_matrix = glm::mat4(1.0f);
		m_matrix = glm::translate(m_matrix, m_pos);
	}

	void Model::update_rotation() {
		m_matrix = glm::rotate(m_matrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_matrix = glm::rotate(m_matrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_matrix = glm::rotate(m_matrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void Model::update_scale() {
		m_matrix = glm::scale(m_matrix, m_scale);
	}

}