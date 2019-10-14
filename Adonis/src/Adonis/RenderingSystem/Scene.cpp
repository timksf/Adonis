#include "pch.h"
#include "Scene.h"


namespace Adonis {

	namespace rendersystem {

		Scene::Scene(SceneType type):
			m_type(type){
		}

		void Scene::add_model(std::shared_ptr<Model> model) {

			AD_CORE_ASSERT(model->initialized(), "Model not initialized, probably forgot to name it");

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
			m_meshgroups[model->mesh_specs()].add_model(model);

			m_model_count++;
		}

		void Scene::draw_init() {
			AD_CORE_ASSERT((m_pipe != nullptr), "Scene: rendering pipeline not initialized");
			m_pipe->activate();
			if (m_type == SceneType::Scene3D) {
				this->set_cam_uniforms();
			}
		}

		void Scene::enable_cam() {
			m_cams[m_active_cam]->enable_input();
			m_cams[m_active_cam]->m_movement_status &= ~MovementDirection::Stationary;
		}

		void Scene::disable_cam() {
			m_cams[m_active_cam]->disable_input();
			m_cams.at(m_active_cam)->reset_movement();
		}

		void Scene::set_resolution(float width, float height) {
			m_cams[m_active_cam]->m_aspectratio = width / height;
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

		void Scene::add_cam(std::unique_ptr<Camera>&& cam, bool auto_select) {
			m_cams.push_back(std::move(cam));
			if(auto_select)
				select_cam(m_cams.size() - 1);
		}

		void Scene::select_cam(uint32_t index) {
			AD_CORE_ASSERT((index >= 0 && index < m_cams.size()), "Index out of bounds: ");
			m_active_cam = index;
		}

		void Scene::set_cam_uniforms() {
			m_pipe->get_param("view")->set_mat4f(m_cams[m_active_cam]->view());
			m_pipe->get_param("projection")->set_mat4f(m_cams[m_active_cam]->projection());
		}

		std::shared_ptr<Model> Scene::model(const char* name) {

			for (auto& group : m_meshgroups) {
				auto model = group.second.model(name);
				if (model) {
					return model;
				}
			}

			return nullptr;
		}

		bool Scene::remove_model(const char* name) {

			for (auto& group : m_meshgroups) {
				if (group.second.remove_model(name)) {
					return true;
				}
			}

			return false;
		}

		MeshGroup& Scene::mesh_group(MeshSpecification spec) {
			return m_meshgroups[spec];
		}

		void MeshGroup::add_model(std::shared_ptr<Model> model) {
			for (auto& m : m_models) {
				AD_CORE_ASSERT(!(model->name() == m->name()), "Model must have unique name");
			}

			m_models.push_back(model);
		}

		void MeshGroup::set_vao(std::unique_ptr<render::VertexArray>&& vao) {
			m_vao = std::move(vao);
		}

		void MeshGroup::use_model(std::shared_ptr<Model> model) {

			m_vao->set_vertex_buffer(model->vbo_id(), 0); //TODO custom bindingindex in case of custom vertex layout
			
			if (model->is_indexed())
				m_vao->set_index_buffer(model->ibo_id());

		}

		std::shared_ptr<Model> MeshGroup::model(const char* name) {

			for (auto& model : m_models) {
				if (model->name() == name)
					return model;
			}

			return nullptr;
		}

		bool MeshGroup::remove_model(const char* name) {

			for (auto& it = m_models.begin(); it != m_models.end(); it++) {
				if ((*it)->name() == name) {
					it = m_models.erase(it);
					return true;
				}

			}

			return false;

		}





	}
}