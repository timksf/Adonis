#include "pch.h"
#include "Scene.h"


namespace Adonis {
	namespace rendersystem {

		Scene::Scene(SceneType type):
			m_type(type){
		}

		void Scene::add_model(std::unique_ptr<Model>&& model) {

			//Create new or add buffers to existing vao
			bool new_vao_needed;
			if (m_meshgroups.empty()) {
				new_vao_needed = true;
			}
			else {
				new_vao_needed = true;
				//check if any existing buffer description matches the description of the new model
				new_vao_needed = !m_meshgroups.count(model->mesh_specs());
			}

			if (new_vao_needed) {
				//create new vao for the model's buffer description
				//if the description doesn't match any existing one, a new mesh group with the id for the new vao is needed
				m_meshgroups[model->mesh_specs()].set_vao(std::move(model->create_vao()));
			}

			//Increase primitive count for mesh group and transfer ownership of the model to it
			//m_meshgroups[model->mesh_specs()].prim_count() += model->primitive_count();
			m_meshgroups[model->mesh_specs()].add_model(std::move(model));
		}

		void Scene::draw_init() {
			AD_CORE_ASSERT((m_pipe != nullptr), "Scene: rendering pipeline not initialized");
			m_pipe->activate();
			if (m_type == SceneType::Scene3D) {
				this->set_cam_uniforms();
			}
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

		void Scene::add_cam(std::unique_ptr<Camera>&& cam) {
			m_cams.push_back(std::move(cam));
		}

		void Scene::select_cam(uint32_t index) {
			AD_CORE_ASSERT(index >= 0 && index < m_cams.size(), "Scene: Index out of bounds");
			m_active_cam = index;
		}

		void Scene::set_cam_uniforms() {
			m_pipe->get_param("view")->set_mat4f(m_cams[m_active_cam]->view());
			m_pipe->get_param("projection")->set_mat4f(m_cams[m_active_cam]->projection());
		}



		void MeshGroup::add_model(std::unique_ptr<Model>&& model) {
			m_models.push_back(std::move(model));
		}

		MeshGroup& Scene::mesh_group(MeshSpecification spec) {
			return m_meshgroups[spec];
		}

		void MeshGroup::set_vao(std::unique_ptr<render::VertexArray>&& vao) {
			m_vao = std::move(vao);
		}

		void MeshGroup::use_model(uint32_t model_index) {

			AD_CORE_ASSERT((model_index >= 0 && model_index < m_models.size()), "Index out of bounds");

			m_active_model = model_index;

			m_vao->set_vertex_buffer(m_models[m_active_model]->vbo_id(), 0); //TODO custom bindingindex in case of custom vertex layout
			
			if (m_models[m_active_model]->is_indexed())
				m_vao->set_index_buffer(m_models[m_active_model]->ibo_id());

		}

		uint32_t MeshGroup::active_model_prim_count() {
			return m_models[m_active_model]->primitive_count();
		}

		auto MeshGroup::active_model_matrix() -> glm::mat4& {
			return m_models[m_active_model]->matrix();
		}


	}
}