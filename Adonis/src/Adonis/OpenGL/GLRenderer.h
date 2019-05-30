#pragma once

#include "Adonis/Core.h"
#include "Adonis/Rendering/Renderer.h"

namespace Adonis {

	namespace render {

		class ADONIS_API GLRenderer : public RenderDevice {
		public:
			GLRenderer(const Color& clear_color);

			~GLRenderer();

			DECLARE_LISTENER(GLRenderer);

			ON_EVENT_DECL_OVERRIDE(WindowResizeEvent);

			GLRenderer(const GLRenderer& other) = delete;

			GLRenderer& operator=(const GLRenderer& other) = delete;

			/**
			*	@brief	Clear the current active framebuffer with the renderer's clear color
			*/
			auto clear_color_buffer(uint32_t buffer_index)->void override;

			auto clear_depth_buffer(float depth)->void override;

			auto drawTriangles(int offset, int count)->void override;

			auto aspect_ratio()->float override;

			auto set_framebuffer(uint32_t id)->void override;

			auto set_viewport(int x0, int y0, uint32_t width, uint32_t height)->void override;

			auto draw(std::shared_ptr<rendersystem::Scene> scene)->void override;

			auto draw(DrawMethod method, DrawMode mode, int offset, int count)->void override;

			/**
			*	@brief			Change the currently acitve rendering pipeline
			*	@param pipe		A pointer to the pipeline, which should be activated
			*/
			auto set_pipeline(std::shared_ptr<RenderPipeline> pipe)->void override;

			/**
			*	@return	A string containing the name of the rendering device
			*/
			inline auto renderer()->std::string override { return m_renderer; };

			/**
			*	@return	A string containing the OpenGL version
			*/
			inline auto version()->std::string override { return m_version; };

			/**
			*	@return	A string containing the version of the OpenGL Shading Language
			*/
			inline auto sl_language_version()->std::string override { return m_glslversion; };

			/**
			*	@return	A string containing the name of the vendor of the active rendering device
			*/
			inline auto vendor()->std::string override { return m_vendor; };


		private:
			std::string m_renderer;
			std::string m_version;
			std::string m_glslversion;
			std::string m_vendor;
			glm::vec2 m_viewport;
			GLuint m_framebuffer{ 0 };
		};

		class ADONIS_API GLTexture2D : public Texture2D {
		public:
			GLTexture2D(int width, int height, const void* data, TexturePixelFormatSized fmt);
			~GLTexture2D() override;

			auto id()->GLuint override;
			auto set_param(TextureParameter param, TextureParamValue value)->void override;

		private:
			GLuint m_id;
		};


		class ADONIS_API GLFramebuffer : public Framebuffer {
		public:
			GLFramebuffer();
			~GLFramebuffer() override;

			auto attach(uint32_t tex_id, FramebufferTextureAttachment attachment_type, int attachment_index)->void override;

			auto activate_color_attachment(int index)->void override;

			auto complete()->bool override;
			auto id()->uint32_t override;
			auto max_color_attachments()->int override;

		private:
			GLuint m_id;
		};

		class ADONIS_API GLUtil {
		public:

			/**
			*	@brief	Check the info log of the shader's compilation
			*/
			static auto check_vert_shader(GLuint shader_id)->void;

			/**
			*	@brief	Check the info log of the shader's compilation
			*/
			static auto check_frag_shader(GLuint shader_id)->void;

			/**
			*	@brief	Check the info log of the program's linking process
			*/
			static auto check_program(GLuint program_id)->void;
		private:
			static auto check(GLuint id, const std::string& type)->void;

		};

		class ADONIS_API GLVertexShader : public VertexShader {
		public:

			/**
			*	@brief			Construct a vertex shader object with according GLSL code
			*	@param code		a string of the GLSL code for the shader
			*/
			GLVertexShader(const std::string& code);

			/**
			*	@brief	Properly releases the underlying OpenGL shader object
			*/
			~GLVertexShader() override;

			/**
			*	@return	the unique id of the shader, which it was assigned to by OpenGl
			*/
			inline auto id()->GLuint override { return m_id; };
			
		private:
			GLuint m_id;
		};

		class ADONIS_API GLFragmentShader : public FragmentShader {
		public:

			/**
			*	@brief			Construct a fragment shader object with according GLSL code
			*	@param code		a string of the GLSL code for the shader
			*/
			GLFragmentShader(const std::string& code);

			/**
			*	@brief	Properly releases the underlying OpenGL shader object
			*/
			~GLFragmentShader() override;

			/**
			*	@return	the unique id of the shader, which it was assigned to by OpenGl
			*/
			inline auto id()->GLuint override { return m_id; };

		private:
			GLuint m_id;
		};


		class ADONIS_API GLRenderPipeline : public RenderPipeline {
		public: 

			/**
			*	@brief					Construct an OpenGL renderpipeline 
			*	@param vertex_shader	an unique pointer to a vertex shader object, which contains the shader code and an id
			*	@param fragment_shader	an unique pointer to a fragment shader object, which contains the shader code and an id
			*/
			GLRenderPipeline(std::unique_ptr<VertexShader> vertex_shader, std::unique_ptr<FragmentShader> frag_shader);

			/**
			*	@brief	Properly deletes the underlying OpenGL program
			*/
			~GLRenderPipeline()	override;

			/**
			*	@brief	Activates the underlying OpenGL program, owned by the pipeline
			*/
			auto activate()const ->void override;

			/**
			*	@brief			Get access to a uniform in a shader program
			*	@param name		the name of the requested uniform
			*	@return			a pointer to the requested param object, may be nullptr
			*/
			auto get_param(const std::string& name)->std::shared_ptr<PipelineParam> override;
			
			/**
			*	@return the id of the underlying OpenGL program, which contains the shaders
			*/
			auto program_id()->GLuint;
		private:
			std::unique_ptr<VertexShader> m_vertex_shader;
			std::unique_ptr<FragmentShader> m_fragment_shader;
			GLuint m_program_id;
			std::vector<std::shared_ptr<PipelineParam>> m_params;
		};

		class ADONIS_API GLPipelineParam : public PipelineParam {
		public:

			/**
			*	@brief				Construct a pipeline parameter, which provides access to a uniform in a shader
			*	@param name			the name of the uniform
			*	@param location		the location of the uniform in the shader
			*	@param program_id	the id of the program, which the shader is attached to
			*/
			GLPipelineParam(const std::string& name, GLint location, GLuint program_id);

			/**
			*	@brief			Set the value of the shader uniform as an int
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_int(int32_t)		->void override;

			/**
			*	@brief			Set the value of the shader uniform as an unsigned int
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_uint(uint32_t)		->void override;

			/**
			*	@brief			Set the value of the shader uniform as a float
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_float(float)		->void override;

			/**
			*	@brief			Set the value of the shader uniform as a double
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_double(double)		->void override;

			/**
			*	@brief			Set the value of the shader uniform as a boolean
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_bool(bool)			->void override;

			/**
			*	@brief			Set the value of the shader uniform as a vector of two booleans
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2b(glm::bvec2)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as vector of two integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2i(glm::ivec2)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as a vector of two unsigned integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2u(glm::uvec2)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as a vector of two floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2f(glm::fvec2)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as a vector of three booleans
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3b(glm::bvec3)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as a vector of three integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3i(glm::ivec3)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as a vector of htree unsigned integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3u(glm::uvec3)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as a vector of three floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3f(glm::fvec3)	->void override;

			/**
			*	@brief		Set the value of the shader uniform as a 3x3 matrix of floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_mat3f(glm::fmat3)	->void override;

			/**
			*	@brief			Set the value of the shader uniform as a 4x4 matrix of floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_mat4f(glm::fmat4)	->void override;

			/**
			*	@return	the name of the uniform, which the pipeline param object grants access to
			*/
			auto name()->std::string;

			/**
			*	@return	the location of the uniform in the shader
			*/
			auto location()->GLint;

		private:
			GLint m_location;
			GLuint m_program_id;
			std::string m_name;
		};


		class ADONIS_API GLVertexBuffer: public VertexBuffer {
		public:

			/**
			*	@brief			Construct an OpenGL vertex buffer object
			*
			*	@param size		the size of the storage to be reserved in bytes
			*	@param data		pointer to the data, may be nullptr if buffer should not be initialized
			*	@param flags	specifies intented usage of the buffer's data store; default flag allows data to be updated through glBufferSubData;
			*					see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml for more information on the flag parameter
			*/
			GLVertexBuffer(int64_t size, const void* data, BufferBit flags);

			/**
			*	@brief Properly deletes the underlying OpenGL buffer object
			*/
			~GLVertexBuffer() override;

			auto id()->GLuint;

			auto bind()->void;

		private:
			GLuint m_id;
		};

		class ADONIS_API GLIndexBuffer: public IndexBuffer {
		public:

			/**
			*   @brief			Construct an OpenGL index buffer object
			*
			*	@param size		the size of the storage to be reserved
			*	@param data		pointer to the data, may be nullptr if buffer should not be initialized
			*	@param flags	specifies intented usage of the buffer's data store; default flag allows data to be updated through glBufferSubData; 
			*					see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml for more information on the flag parameter
			*/
			GLIndexBuffer(int64_t size, const void* data, BufferBit flags);

			/**
			*	@brief Properly deletes the underlying OpenGL buffer object
			*/
			~GLIndexBuffer() override;

			auto id()->GLuint;

			auto bind()->void;

		private:
			GLuint m_id;
		};

		class ADONIS_API GLVertexAttrib : public VertexAttrib {
		public:

			/**
			*	@brief				Construct a GLVertexAttrib which specifies the format for a vertex array attribute
			*
			*	@param index		the attribute's index, typically just an index from a list
			*	@param offset		the offset of the attribute from the beginning of a vertex in bytes
			*	@param type			the type of the attribute 
			*	@param normalized	if true, integer values will be normalized via integer normalization
			*/
			GLVertexAttrib(GLuint index, GLuint offset, VertexType type, GLuint size, GLboolean normalized);

			inline auto index()->uint32_t override { return m_index; };
			inline auto offset()->uint32_t override { return m_offset; };
			inline auto type()->VertexType override { return m_type; };
			inline auto api_type()->uint32_t override { return m_api_type; };
			inline auto size()->uint32_t override { return m_size; };

			inline auto normalized()->bool override { return m_normalized; };

		private:
			GLuint m_index;
			GLuint m_offset;
			GLenum m_api_type;
			GLuint m_size;
			GLboolean m_normalized;
			VertexType m_type;
		};

		class ADONIS_API GLVertexArrayDesc : public VertexArrayDesc {
		public:

			/**
			*	@brief				Construct a vertex array description, which describes the structure of a vertex buffer 
			*
			*	@param baseoffset	the offset of the vertex buffer data from the beginning of the actual vertex buffer object in bytes
			*	@param stride		the number of bytes between vertex elements, usually the size of the vertex type used in the buffer
			*/
			GLVertexArrayDesc(std::vector<std::unique_ptr<VertexAttrib>>&& attribs, GLuint baseoffset, GLsizei stride);

			/**
			*	@brief Deleted copy constructor to restrict copy constructing, because of the owned unique pointers
			*/
			GLVertexArrayDesc(const GLVertexArrayDesc&) = delete;

			/**
			*	@return	Deleted copy assignment operator to restric copy assignment, because of stored unique pointers 
			*/
			GLVertexArrayDesc& operator=(const GLVertexArrayDesc&) = delete;

			/**
			*	@return	Return a reference to the stored attribute formats
			*/
			auto attribs()->std::vector<std::unique_ptr<VertexAttrib>>& override;

			/**
			*	@return	the offset of the vertex buffer data from the beginning of the actual vertex buffer object in bytes
			*/
			inline auto baseoffset()->GLuint override { return m_baseoffset; };

			/**
			*	@return	the number of bytes between the vertex elements, usually the size of the vertex type
			*/
			inline auto stride()->GLuint override { return m_stride; };

			auto add_attrib(std::unique_ptr<VertexAttrib>&& attrib)->void override;

			auto add_attrib(VertexType type, uint32_t number, int custom_index, int custom_offset)->void override;

		private:
			GLsizei m_stride;
			GLuint m_baseoffset;
			std::vector<std::unique_ptr<VertexAttrib>> m_attribs;
		};

		class ADONIS_API GLVertexArray : public VertexArray {
		public:

			GLVertexArray() = delete;

			/**
			*	@brief Create the underlying OpenGL vertex array object and store the id
			*/
			GLVertexArray(std::shared_ptr<VertexArrayDesc>);

			/**
			*	@brief Properly release the underlying OpenGL vertex array object
			*/
			~GLVertexArray() override;

			/**
			*	@brief Prohibit copy construction because m_vbuffers member contains unique_ptr s
			*/
			GLVertexArray(const GLVertexArray& other) = delete;

			/**
			*	@brief Prohibit copy assignment because m_vbuffers member contains unique_ptr s
			*/
			GLVertexArray& operator=(const GLVertexArray& other) = delete;

			/**
			*	@brief					Add a buffer to the vertex array object with a corresponding description of the buffer's structure
			*	
			*	@param	vbo				the name/id of the vertex buffer object
			*	@param	attrib_desc		the description of the buffer's structure, basically a list of attribute formats
			*/
			//auto add_buffer(GLuint vbo, std::unique_ptr<VertexArrayDesc> attrib_desc)->bool override;

			/**
			*	@brief	Enable underlying OpenGL vertex array object
			*/
			auto bind()->void override;

			auto id()->uint32_t override;

			auto set_buffer(GLuint bufferid, GLuint bindingindex, int32_t custom_baseoffset, int32_t custom_stride)->void override;

			auto add_desc(std::shared_ptr<VertexArrayDesc> desc, bool increase_bindingindex, bool overwrite_existing_attribs)->uint32_t override;

			auto desc()->std::shared_ptr<VertexArrayDesc> override;

			auto set_index_buffer(uint32_t idx_buffer_id)->void override;

		protected:

			auto add_attrib_to_underlying_obj(uint32_t where)->void override;

		private:
			uint32_t m_current_bindingindex;
			std::shared_ptr<VertexArrayDesc> m_desc;
			GLuint m_id;
		};


	}

}
