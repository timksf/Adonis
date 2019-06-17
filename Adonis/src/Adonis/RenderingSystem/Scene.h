#pragma once


#include "Adonis/Core.h"
#include <memory>
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {


	namespace rendersystem {

		enum class SceneType {
			Scene2D = 0,
			Scene3D
		};

		class MeshGroup {
		public:

			auto set_vao(std::unique_ptr<render::VertexArray>&& vao)->void;

			inline auto activate()const->void { m_vao->use(); };

			auto add_model(std::unique_ptr<Model>&& model)->void;

			auto use_model(uint32_t model_index)->void;

			auto active_model_prim_count()->uint32_t;

			auto active_model_matrix()->glm::mat4 &;

			inline auto number_of_models()->size_t { return m_models.size(); };

		private:
			uint32_t m_active_model{ 0 };
			std::unique_ptr<render::VertexArray> m_vao{ nullptr };
			std::vector<std::unique_ptr<Model>> m_models;
		};

		class Scene {
		public:

			Scene(SceneType type = SceneType::Scene2D);

			auto select_cam(uint32_t index)->void;

			auto add_cam(std::unique_ptr<Camera>&& cam, bool auto_select = false)->void;

			auto add_model(std::unique_ptr<Model>&& model)->void;

			auto mesh_specs()->std::vector<MeshSpecification>;

			auto mesh_group(MeshSpecification spec)->MeshGroup&;

			auto draw_init()->void;

			auto enable_cam()->void;

			auto disable_cam()->void;

			auto set_resolution(float width, float height)->void;

			inline auto set_pipe(std::shared_ptr<render::RenderPipeline> pipe) { m_pipe = pipe; };

			inline auto pipe()->std::shared_ptr<render::RenderPipeline> { return m_pipe; };

			inline auto type()->SceneType { return m_type; };

			inline auto cam_info()->CamInfo { return m_cam_info; };

			inline auto active_cam()->uint32_t { return m_active_cam; };

			inline auto number_of_cams()->uint32_t { return m_cams.size(); };

			template<typename... Args>
			inline void add_cam(Args&&... args, bool auto_select = false) {
				m_cams.push_back(std::make_unique<Camera>(std::forward<Args>(args)...));
				if(auto_select)
					select_cam(m_cams.size() - 1);
			}

		private:
			auto set_cam_uniforms()->void;
			auto update_cam_info()->void;


			SceneType m_type;
			uint32_t m_active_cam{ 0 };
			std::vector<std::unique_ptr<Camera>> m_cams;
			std::unordered_map<MeshSpecification, MeshGroup, Hasher<MeshSpecification>> m_meshgroups;
			std::shared_ptr<render::RenderPipeline> m_pipe{ nullptr };
			CamInfo m_cam_info;
			//std::vector<std::unique_ptr<render::VertexArray>> m_vaos;
		};

	}
}