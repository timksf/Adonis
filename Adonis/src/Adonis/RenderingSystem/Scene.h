#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Adonis/Core.h"
#include "Model.h"
#include "Mesh.h"
#include "Camera.h"
#include "Adonis/Rendering/Renderer.h"

#include <unordered_map>

namespace Adonis {


	namespace rendersystem {

		enum class ADONIS_API SceneType {
			Scene2D = 0,
			Scene3D
		};

		class ADONIS_API MeshGroup {
		public:

			auto set_vao(std::unique_ptr<render::VertexArray>&& vao)->void;

			inline auto activate()const->void { m_vao->use(); };

			auto add_model(std::shared_ptr<Model> model)->void;

			auto remove_model(const char* name)->bool;

			auto use_model(std::shared_ptr<Model> model)->void;

			auto model(const char* name)->std::shared_ptr<Model>;

			inline auto models()->std::vector<std::shared_ptr<Model>>& { return m_models; };

			inline auto number_of_models()->size_t { return m_models.size(); };

		private:
			uint32_t m_active_model{ 0 };
			std::unique_ptr<render::VertexArray> m_vao{ nullptr };
			std::vector<std::shared_ptr<Model>> m_models;
		};

		class ADONIS_API Scene {
		public:

			Scene(SceneType type = SceneType::Scene2D);

			Scene(const Scene&) = delete;

			Scene& operator=(const Scene&) = delete;

			auto select_cam(uint32_t index)->void;

			auto add_cam(std::unique_ptr<Camera>&& cam, bool auto_select = false)->void;

			auto add_model(std::shared_ptr<Model> model)->void;

			auto remove_model(const char* name)->bool;

			auto mesh_specs()->std::vector<MeshSpecification>;

			auto mesh_group(MeshSpecification spec)->MeshGroup&;

			auto draw_init()->void;

			auto enable_cam()->void;

			auto disable_cam()->void;

			auto set_resolution(float width, float height)->void;

			inline void test() { m_cams[m_active_cam]->m_enable_zoom = true; };

			inline auto set_pipe(std::shared_ptr<render::RenderPipeline> pipe) { m_pipe = pipe; };

			inline auto pipe()->std::shared_ptr<render::RenderPipeline> { return m_pipe; };

			inline auto type()->SceneType { return m_type; };

			inline auto active_cam_idx()->uint32_t { return m_active_cam; };

			inline auto active_cam()->std::shared_ptr<Camera> { return m_cams[m_active_cam]; };

			inline auto number_of_cams()->uint32_t { return m_cams.size(); };

			inline auto number_of_models()->uint32_t { return m_model_count; };

			template<typename... Args>
			inline auto add_cam(Args&&... args, bool auto_select = false)->void {
				m_cams.push_back(std::make_shared<Camera>(std::forward<Args>(args)...));
				if(auto_select)
					select_cam(m_cams.size() - 1);
			}

			inline auto add_default_cam(bool auto_select = false)->void {
				m_cams.emplace_back(new Camera());
				if (auto_select)
					select_cam(m_cams.size() - 1);
			}


		private:
			auto set_cam_uniforms()->void;

			uint32_t m_model_count{ 0 };
			SceneType m_type;
			uint32_t m_active_cam{ 0 };
			std::vector<std::shared_ptr<Camera>> m_cams;
			std::unordered_map<MeshSpecification, MeshGroup, hash<MeshSpecification>> m_meshgroups;
			std::shared_ptr<render::RenderPipeline> m_pipe{ nullptr };
		};

	}
}

#endif