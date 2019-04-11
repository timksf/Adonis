#include "pch.h"
#include "Scene.h"


namespace Adonis {
	namespace rendersystem {

		void MeshGroup::add_model(std::unique_ptr<Model>&& model) {
			m_models.push_back(std::move(model));
		}


		void Scene::add_model(std::unique_ptr<Model>&& model) {

			//Create new or add buffers to existing vao
			bool new_vao_needed;
			if (m_vaos.empty()) {
				new_vao_needed = true;
			}
			else {
				new_vao_needed = true;
				for (auto& vao : m_vaos) {
					//check if any existing buffer description matches the description of the new model
					if (model->mesh_specs().buffer_desc() == vao->desc()) {
						new_vao_needed = false;
					}
				}
			}

			if (new_vao_needed) {
				//create new vao for the model's buffer description
				auto temp_vao = render::VertexArray::create(model->mesh_specs().buffer_desc());
				//if the description doesn't match any existing one, a new mesh group with the id for the new vao is needed
				m_meshgroups[model->mesh_specs()].vao_id() = temp_vao->id();
				m_vaos.push_back(std::move(temp_vao));
			}

			//Increase primitive count for mesh group and transfer ownership of the model to it
			m_meshgroups[model->mesh_specs()].prim_count() += model->primitive_count();
			m_meshgroups[model->mesh_specs()].add_model(std::move(model));
		}

		std::vector<MeshSpecification> Scene::mesh_specs() {
			//Extract keys from model map
			std::vector<MeshSpecification> keys;
			keys.reserve(m_meshgroups.size());
			for (auto& kv : m_meshgroups) {
				keys.push_back(kv.first);
			}
			return keys;
		}

		MeshGroup& Scene::mesh_group(MeshSpecification spec) {
			return m_meshgroups[spec];
		}

	}
}