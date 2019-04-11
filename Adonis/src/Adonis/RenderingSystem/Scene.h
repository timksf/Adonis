#pragma once


#include "Adonis/Core.h"
#include <memory>
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {


	namespace rendersystem {

		class MeshGroup {
		public:

			inline auto prim_count()->uint32_t& { return m_prim_count; };
			inline auto vao_id()->uint32_t& { return m_vao_id; };

			auto add_model(std::unique_ptr<Model>&& model)->void;

		private:
			uint32_t m_prim_count;
			uint32_t m_vao_id;
			std::vector<std::unique_ptr<Model>> m_models;
		};

		class Scene {
		public:

			Scene() {};

			auto select_cam(uint32_t index)->bool {};

			auto add_model(std::unique_ptr<Model>&& model)->void;

			auto mesh_specs()->std::vector<MeshSpecification>;

			auto mesh_group(MeshSpecification spec)->MeshGroup&;

			auto add_cam(std::unique_ptr<Camera>&& cam)->void {};

			template<typename... Args>
			inline void add_cam(Args&&... args) {
				m_cams.push_back(std::make_unique<Camera>(std::forward<Args>(args)...));
			}

		private:
			uint32_t m_active_cam;
			std::vector<std::unique_ptr<Camera>> m_cams;
			std::unordered_map<MeshSpecification, MeshGroup, Hasher<MeshSpecification>> m_meshgroups;
			std::vector<std::unique_ptr<render::VertexArray>> m_vaos;
		};

	}
}