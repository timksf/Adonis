#include "pch.h"
#include "Scene.h"



namespace Adonis::rendersystem {

	void MeshGroup::add_model(std::unique_ptr<Model>&& model) {
		m_models.push_back(std::move(model));
	}


	void Scene::add_model(std::unique_ptr<Model>&& model) {

		//Create new or add buffers to existing vao
		if (m_vaos.empty()) {
			//auto vao = render::VertexArray::create();
		}


		m_models[model->mesh_specs()].prim_count() += model->primitive_count();
		m_models[model->mesh_specs()].add_model(std::move(model));
	}

}