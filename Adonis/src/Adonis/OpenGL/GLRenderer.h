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
			auto set_pipeline(std::shared_ptr<RenderPipeline> pipe)->void override;

			inline auto renderer()->std::string override { return m_renderer; };
			inline auto version()->std::string override { return m_version; };
			inline auto sl_language_version()->std::string override { return m_glslversion; };
			inline auto vendor()->std::string override { return m_vendor; };

		private:
			std::string m_renderer;
			std::string m_version;
			std::string m_glslversion;
			std::string m_vendor;
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
			std::vector<std::shared_ptr<PipelineParam>> m_params;
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


		class ADONIS_API GLVertexBuffer: public VertexBuffer {
		public:

			GLVertexBuffer(int64_t size, const void* data, GLenum usage);
			~GLVertexBuffer() override;

		private:
			GLuint m_id;
		};

		class ADONIS_API GLIndexBuffer: public IndexBuffer {
		public:

			GLIndexBuffer(int64_t size, const void* data, GLenum usage);
			~GLIndexBuffer() override;

		private:
			GLuint m_id;
		};

		class ADONIS_API GLVertexAttrib : public VertexAttrib {
		public:

			GLVertexAttrib(GLuint, GLuint, GLuint, GLsizei, GLenum, GLuint, GLboolean);

			GLuint _binding;
			GLuint _index;
			GLuint _offset;
			GLsizei _stride;
			GLenum _type;
			GLuint _size;
			GLboolean _normalized;
		};

		class ADONIS_API GLVertexArrayDesc : public VertexArrayDesc {
		public:
			GLVertexArrayDesc(std::vector<std::unique_ptr<VertexAttrib>> attribs, GLuint baseoffset);

			//dont allow copy operations
			GLVertexArrayDesc(const GLVertexArrayDesc&) = delete;
			GLVertexArrayDesc& operator=(const GLVertexArrayDesc&) = delete;

			auto attribs()->std::vector<std::unique_ptr<VertexAttrib>>& override;
			inline auto baseoffset()->GLuint override { return m_baseoffset; };
		private:
			GLuint m_baseoffset;
			std::vector<std::unique_ptr<VertexAttrib>> m_attribs;
		};

		class ADONIS_API GLVertexArray : VertexArray {
		public:

			GLVertexArray();
			~GLVertexArray() override;
			auto add_buffer(GLuint id, std::shared_ptr<VertexArrayDesc> attrib_desc)->bool override;

		private:
			std::unordered_map<GLuint, std::shared_ptr<VertexArrayDesc>> m_vbuffers;
			GLuint m_id;
		};


	}

}
