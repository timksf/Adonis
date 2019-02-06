#pragma once

#include "Adonis/Core.h"
#include "Adonis/RenderDevice.h"

namespace Adonis {

	namespace render {

		class ADONIS_API GLRenderer : public RenderDevice {
		public:
			GLRenderer(const Color& clear_color);

			~GLRenderer();

			DECLARE_LISTENER(GLRenderer);

			ON_EVENT_DECL_OVERRIDE(PreRenderEvent);
			ON_EVENT_DECL_OVERRIDE(RenderEvent);
			ON_EVENT_DECL_OVERRIDE(UpdateEvent);

			auto clear()->void override;


		};

		class ADONIS_API GLUtil {
		public:
			static auto check_vert_shader(GLuint shader_id)->void;
			static auto check_frag_shader(GLuint shader_id)->void;
			static auto check_program(GLuint program_id)->void;
		private:
			static auto check(GLuint id, const std::string& type)->void;

		};

		class ADONIS_API GLVertexShader : public VertexShader {
		public:

			GLVertexShader(const std::string& code);
			~GLVertexShader() override;

			inline auto id()->GLuint override { return m_id; };
			
		private:
			GLuint m_id;
		};

		class ADONIS_API GLFragmentShader : public FragmentShader {
		public:

			GLFragmentShader(const std::string& code);
			~GLFragmentShader() override;

			inline auto id()->GLuint override { return m_id; };

		private:
			GLuint m_id;
		};

		class GLPipelineParam;

		class ADONIS_API GLRenderPipeline : public RenderPipeline {
		public:

			GLRenderPipeline(std::unique_ptr<VertexShader> vertex_shader, std::unique_ptr<FragmentShader> frag_shader);
			~GLRenderPipeline()	override;

			auto get_param(const std::string& name)->std::shared_ptr<PipelineParam>;

			auto program_id()->GLuint;
		private:
			std::unique_ptr<VertexShader> m_vertex_shader;
			std::unique_ptr<FragmentShader> m_fragment_shader;
			GLuint m_program_id;
			std::vector<std::shared_ptr<GLPipelineParam>> m_params;
		};

		class ADONIS_API GLPipelineParam : public PipelineParam {
		public:

			GLPipelineParam(const std::string& name, GLint location, GLuint proragm_id);

			auto set_int(int32_t)		->void override;
			auto set_uint(uint32_t)		->void override;
			auto set_float(float)		->void override;
			auto set_double(double)		->void override;
			auto set_bool(bool)			->void override;
			auto set_vec2b(glm::bvec2)	->void override;
			auto set_vec2i(glm::ivec2)	->void override;
			auto set_vec2u(glm::uvec2)	->void override;
			auto set_vec2f(glm::fvec2)	->void override;
			auto set_vec3b(glm::bvec3)	->void override;
			auto set_vec3i(glm::ivec3)	->void override;
			auto set_vec3u(glm::uvec3)	->void override;
			auto set_vec3f(glm::fvec3)	->void override;
			auto set_mat3f(glm::fmat3)	->void override;
			auto set_mat4f(glm::fmat4)	->void override;

			auto name()->std::string;
			auto location()->GLint;

		private:
			GLint m_location;
			GLuint m_program_id;
			std::string m_name;
		};

	}

}
