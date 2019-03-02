#pragma once
#include "pch.h"
#include "Adonis/Core.h"
#include "GLRenderer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Adonis/Math/Math.h"

namespace Adonis {

	namespace render {


		GLRenderer::GLRenderer(const Color& clear_color) : RenderDevice(clear_color) {
			ON_EVENT_BIND(WindowResizeEvent, GLRenderer);
			m_renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
			m_glslversion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
			m_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
			m_vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		}	

		GLRenderer::~GLRenderer() {

		}

		std::unique_ptr<RenderDevice> RenderDevice::create(const Color& clear_color) {
			return std::make_unique<GLRenderer>(clear_color);
		}

		void GLRenderer::clear_color_buffer(uint32_t buffer_index) {
			glClearNamedFramebufferfv(m_framebuffer, GL_COLOR, buffer_index, &clear_color.r());
		}

		void GLRenderer::clear_depth_buffer(float depth) {
			glClearNamedFramebufferfv(m_framebuffer, GL_DEPTH, 0, &depth);
		}

		void GLRenderer::drawTriangles(int offset, int count) {
			glDrawArrays(GL_TRIANGLES, offset, count);
		}

		void GLRenderer::set_framebuffer(uint32_t id) {
			m_framebuffer = id;
			glBindFramebuffer(GL_FRAMEBUFFER, id);
		}

		void GLRenderer::set_pipeline(std::shared_ptr<RenderPipeline> pipe) {
			pipe->activate();
		}

		void GLRenderer::set_viewport(int x0, int y0, uint32_t width, uint32_t height) {
			m_viewport = { width, height };
			glViewport(x0, y0, width, height);
		}

		float GLRenderer::aspect_ratio() {
			return static_cast<float>(m_viewport.x / m_viewport.y);
		}

		void GLRenderer::on_WindowResizeEvent(const event_ptr_t<WindowResizeEvent>& e) {
			m_viewport = { e->width(), e->height() };
			glViewport(0, 0, e->width(), e->height());
		}

		/*
			SHADER
		*/

		void GLUtil::check_vert_shader(GLuint shader_id) {
			GLUtil::check(shader_id, "Vertex");
		}

		void GLUtil::check_frag_shader(GLuint shader_id) {
			GLUtil::check(shader_id, "Fragment");
		}

		void GLUtil::check_program(GLuint prog_id) {
			GLUtil::check(prog_id, "PROGRAM");
		}


		void GLUtil::check(GLuint id, const std::string& type) {
			int success;
			char infoLog[1024];
			if (type == "PROGRAM") {
				glGetProgramiv(id, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(id, 1024, NULL, infoLog);
					AD_CORE_ERROR("Linking error in shader program: ");
					AD_CORE_ERROR(infoLog);
					AD_CORE_ERROR(" ------------------------------------------------------- ");
				}
			}
			else {
				glGetShaderiv(id, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(id, 1024, NULL, infoLog);
					AD_CORE_ERROR("Compile error in shader of _type: {0}", type);
					AD_CORE_ERROR(infoLog);
					AD_CORE_ERROR(" ------------------------------------------------------- ");
				}
			}
		}


		GLVertexShader::GLVertexShader(const std::string& code) : VertexShader(code) {
			const char* code_c = code.c_str();
			m_id = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(m_id, 1, &code_c, NULL);
			glCompileShader(m_id);
			GLUtil::check_vert_shader(m_id);
		}

		GLVertexShader::~GLVertexShader() {
			glDeleteShader(m_id);
		}

		GLFragmentShader::GLFragmentShader(const std::string& code) : FragmentShader(code) {
			const char* code_c = code.c_str();
			m_id = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(m_id, 1, &code_c, NULL);
			glCompileShader(m_id);
			GLUtil::check_frag_shader(m_id);
		}

		GLFragmentShader::~GLFragmentShader() {
			glDeleteShader(m_id);
		}

		std::unique_ptr<VertexShader> VertexShader::create(const std::string& code) {
			return std::make_unique<GLVertexShader>(code);
		}

		std::unique_ptr<FragmentShader> FragmentShader::create(const std::string& code) {
			return std::make_unique<GLFragmentShader>(code);
		}

		/*
			Render Pipeline
		*/

		std::shared_ptr<RenderPipeline> RenderPipeline::create(std::unique_ptr<VertexShader> vert_shader, std::unique_ptr<FragmentShader> frag_shader) {
			return std::make_shared<GLRenderPipeline>(std::move(vert_shader), std::move(frag_shader));
		}

		GLRenderPipeline::GLRenderPipeline(std::unique_ptr<VertexShader> vertex_shader, std::unique_ptr<FragmentShader> frag_shader) :
			m_vertex_shader(std::move(vertex_shader)),
			m_fragment_shader(std::move(frag_shader))
		{
			m_program_id = glCreateProgram();
			glAttachShader(m_program_id, m_vertex_shader->id());
			glAttachShader(m_program_id, m_fragment_shader->id());
			glLinkProgram(m_program_id);
			GLUtil::check_program(m_program_id);
		}

		GLRenderPipeline::~GLRenderPipeline() {
			glDeleteProgram(m_program_id);
		}

		GLuint GLRenderPipeline::program_id() {
			return m_program_id;
		}

		void GLRenderPipeline::activate()const {
			glUseProgram(m_program_id);
		}

		std::shared_ptr<PipelineParam> GLRenderPipeline::get_param(const std::string& name) {
			auto param = std::find_if(m_params.begin(), m_params.end(), [&](std::shared_ptr<PipelineParam> p) { return p->name() == name; });
			if (param != m_params.end()) {
				return *param;
			}
			else {
				GLint loc = glGetUniformLocation(m_program_id, name.c_str());
				if (loc != -1) {
					m_params.push_back(std::make_shared<GLPipelineParam>(name, loc, m_program_id));
					return *(m_params.end() - 1);
				}
				else {
					AD_CORE_WARN("Pipeline param: \"{0}\" does not exist", name);
					AD_CORE_WARN(loc);
				}
			}
			return nullptr;
		}

		std::unique_ptr<RenderPipeline> RenderPipeline::test_pipeline_2D() {
			return std::make_unique<GLRenderPipeline>(
				std::make_unique<GLVertexShader>(
#include "Adonis/OpenGL/Shaders/2DTest.vert"
					),
				std::make_unique<GLFragmentShader>(
#include "Adonis/OpenGL/Shaders/2DTest.frag"
					)
				);
		}

		std::unique_ptr<RenderPipeline> RenderPipeline::test_pipeline_3D() {
			return std::make_unique<GLRenderPipeline>(
				std::make_unique<GLVertexShader>(
#include "Adonis/OpenGL/Shaders/3DTest.vert"
					),
				std::make_unique<GLFragmentShader>(
#include "Adonis/OpenGL/Shaders/3DTest.frag"
					)
				);
		}

		/*
			Pipeline param
		*/

		GLPipelineParam::GLPipelineParam(const std::string& name, GLint location, GLuint program_id) :
			m_name(name),
			m_location(location),
			m_program_id(program_id)
		{

		}

		std::string GLPipelineParam::name() {
			return m_name;
		}

		GLint GLPipelineParam::location() {
			return m_location;
		}

		void GLPipelineParam::set_int(int32_t v) {
			glUseProgram(m_program_id);
			glUniform1i(m_location, v);
		}
		void GLPipelineParam::set_uint(uint32_t v) {
			glUseProgram(m_program_id);
			glUniform1ui(m_location, v);
		}
		void GLPipelineParam::set_bool(bool v) {
			glUseProgram(m_program_id);
			glUniform1i(m_location, v);
		}
		void GLPipelineParam::set_float(float v) {
			glUseProgram(m_program_id);
			glUniform1f(m_location, v);
		}
		void GLPipelineParam::set_double(double v) {
			glUseProgram(m_program_id);
			glUniform1d(m_location, v);
		}
		void GLPipelineParam::set_vec2b(glm::bvec2 v) {
			glUseProgram(m_program_id);
			glUniform2i(m_location, v[0], v[1]);
		}
		void GLPipelineParam::set_vec2i(glm::ivec2 v) {
			glUseProgram(m_program_id);
			glUniform2i(m_location, v[0], v[1]);
		}
		void GLPipelineParam::set_vec2u(glm::uvec2 v) {
			glUseProgram(m_program_id);
			glUniform2uiv(m_location, 2, &v[0]);
		}
		void GLPipelineParam::set_vec2f(glm::fvec2 v) {
			glUseProgram(m_program_id);
			glUniform2fv(m_location, 2, &v[0]);
		}
		void GLPipelineParam::set_vec3b(glm::bvec3 v) {
			glUseProgram(m_program_id);
			glUniform3i(m_location, v[0], v[1], v[2]);
		}
		void GLPipelineParam::set_vec3i(glm::ivec3 v) {
			glUseProgram(m_program_id);
			glUniform3i(m_location, v[0], v[1], v[2]);
		}
		void GLPipelineParam::set_vec3u(glm::uvec3 v) {
			glUseProgram(m_program_id);
			glUniform3ui(m_location, v[0], v[1], v[2]);
		}
		void GLPipelineParam::set_vec3f(glm::fvec3 v) {
			glUseProgram(m_program_id);
			glUniform3f(m_location, v[0], v[1], v[2]);
		}
		void GLPipelineParam::set_mat3f(glm::fmat3 v) {
			glUseProgram(m_program_id);
			glUniformMatrix4fv(m_location, 1, false, glm::value_ptr(v[0]));
		}
		void GLPipelineParam::set_mat4f(glm::fmat4 v) {
			glUseProgram(m_program_id);
			glUniformMatrix4fv(m_location, 1, false, glm::value_ptr(v[0]));
		}

		/*
			Buffers
		*/

		uint32_t Buffer::buffer_bit_lookup[Buffer::NUMBER_OF_BUFFER_BITS] = { GL_DYNAMIC_STORAGE_BIT, GL_MAP_READ_BIT, GL_MAP_WRITE_BIT };

		/*
			Vertex buffer
		*/

		GLVertexBuffer::GLVertexBuffer(int64_t size, const void* data, BufferBit flags) : VertexBuffer() {
			glCreateBuffers(1, &m_id);
			std::stringstream ss;
			ss << std::hex << std::showbase << static_cast<uint32_t>(flags);
			ss.str(std::string());
			GLbitfield glflags = 0;
			for (uint32_t i = 0; i < Buffer::NUMBER_OF_BUFFER_BITS; i++) {
				if ((i & static_cast<uint32_t>(flags)) == i) {
					glflags |= Buffer::buffer_bit_lookup[i];
				}
			}
			ss << std::hex << static_cast<uint32_t>(glflags);
			glNamedBufferStorage(m_id, size, data, glflags);
		}

		GLVertexBuffer::~GLVertexBuffer() {
			glDeleteBuffers(1, &m_id);
		}

		GLuint GLVertexBuffer::id() {
			return m_id;
		}

		void GLVertexBuffer::bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}

		std::unique_ptr<VertexBuffer> VertexBuffer::create(int64_t size, const void* data, BufferBit flags) {
			return std::make_unique<GLVertexBuffer>(size, data, flags);
		}

		/*
			Index buffer
		*/

		GLIndexBuffer::GLIndexBuffer(int64_t size, const void* data, BufferBit flags) : IndexBuffer() {
			glCreateBuffers(1, &m_id);
			std::stringstream ss;
			ss << std::hex << std::showbase << static_cast<uint32_t>(flags);
			ss.str(std::string());
			GLbitfield glflags = 0;
			for (uint32_t i = 0; i < Buffer::NUMBER_OF_BUFFER_BITS; i++) {
				if ((i & static_cast<uint32_t>(flags)) == 1) {
					glflags |= Buffer::buffer_bit_lookup[i];
				}
			}
			ss << std::hex << static_cast<uint32_t>(glflags);
			glNamedBufferStorage(m_id, size, data, glflags);
		}

		GLIndexBuffer::~GLIndexBuffer() {
			glDeleteBuffers(1, &m_id);
		}

		GLuint GLIndexBuffer::id() {
			return m_id;
		}

		void GLIndexBuffer::bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}

		std::unique_ptr<IndexBuffer> IndexBuffer::create(int64_t size, const void* data, BufferBit flags) {
			return std::make_unique<GLIndexBuffer>(size, data, flags);
		}

		/*
			Vertex Attributes
		*/

		uint32_t VertexBuffer::vertex_type_mapping[VertexBuffer::NUMBER_OF_VERTEX_TYPES] = { GL_FLOAT, GL_HALF_FLOAT, GL_DOUBLE, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT };

		GLVertexAttrib::GLVertexAttrib(GLuint index, GLuint offset, GLenum type, GLuint size, GLboolean normalized) {
			_index = index;
			_offset = offset;
			_type = type;
			_size = size;
			_normalized = normalized;
		}


		std::unique_ptr<VertexAttrib> VertexAttrib::create(uint32_t index, uint32_t offset, VertexType type, uint32_t size) {
			bool normalized = (static_cast<std::underlying_type_t<VertexType>>(type) > 8);
			return std::make_unique<GLVertexAttrib>(index, offset, VertexBuffer::vertex_type_mapping[static_cast<std::underlying_type_t<VertexType>>(type)], size, static_cast<GLboolean>(normalized));
		}

		/*
			Vertex Array/Buffer Description
		*/

		GLVertexArrayDesc::GLVertexArrayDesc(std::vector<std::unique_ptr<VertexAttrib>>&& attribs, GLuint baseoffset, GLsizei stride) :
			m_attribs(std::move(attribs)) {
			m_baseoffset = baseoffset;
			m_stride = stride;
		}

		std::vector<std::unique_ptr<VertexAttrib>>& GLVertexArrayDesc::attribs() {
			return m_attribs;
		}

		std::unique_ptr<VertexArrayDesc> VertexArrayDesc::create(std::vector<std::unique_ptr<VertexAttrib>>&& attribs, uint32_t baseoffset, uint32_t stride) {
			return std::make_unique<GLVertexArrayDesc>(std::move(attribs), baseoffset, stride);
		}

		/*
			Vertex Array Object
		*/

		GLVertexArray::GLVertexArray() {
			glCreateVertexArrays(1, &m_id);
		}

		GLVertexArray::~GLVertexArray() {
			glDeleteVertexArrays(1, &m_id);
		}

		std::unique_ptr<VertexArray> VertexArray::create() {
			return std::make_unique<GLVertexArray>();
		}

		bool GLVertexArray::add_buffer(GLuint vbo, std::unique_ptr<VertexArrayDesc> desc) {
			m_vbuffers.insert(std::make_pair(vbo, std::move(desc)));
			GLuint bindingindex = static_cast<uint32_t>(m_vbuffers.size() - 1);
			glVertexArrayVertexBuffer(m_id, bindingindex, vbo, m_vbuffers[vbo]->baseoffset(), m_vbuffers[vbo]->stride());
			for (auto& attrib : m_vbuffers[vbo]->attribs()) {
				auto glattrib = dynamic_cast<GLVertexAttrib*>(attrib.get());
				glEnableVertexArrayAttrib(m_id, glattrib->_index);
				glVertexArrayAttribFormat(m_id, glattrib->_index, glattrib->_size, glattrib->_type, glattrib->_normalized, glattrib->_offset);
				glVertexArrayAttribBinding(m_id, glattrib->_index, bindingindex);
			}
			return true;
		}

		void GLVertexArray::bind() {
			glBindVertexArray(m_id);
		}

		/**
		* Texture
		*/

		uint32_t Texture::sized_pixel_format_lookup[Texture::NUMBER_OF_SIZED_PIXEL_FORMATS] = { GL_R3_G3_B2, GL_RGB8, GL_RGBA4, GL_RGBA8, GL_DEPTH_COMPONENT16};
		uint32_t Texture::tex_param_lookup[Texture::NUMBER_OF_TEX_PARAMS] = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER };
		uint32_t Texture::tex_param_value_lookup[Texture::NUMBER_OF_TEX_PARAM_VALUES] = {GL_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE};
		uint32_t Texture::pixel_format_lookup[Texture::NUMBER_OF_PIXEL_FORMATS] = { GL_RGB, GL_RGBA, GL_BGR, GL_BGRA };
		uint32_t Texture::pixel_datatype_lookup[Texture::NUMBER_OF_PIXEL_DATATYPES] = { GL_SHORT, GL_UNSIGNED_SHORT, GL_BYTE, GL_UNSIGNED_BYTE, GL_INT, GL_UNSIGNED_INT };	

		GLTexture2D::GLTexture2D(int width, int height, const void* data, TexturePixelFormatSized fmt) {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
			glTextureStorage2D(m_id, 1, Texture::sized_pixel_format_lookup[static_cast<uint32_t>(fmt)], width, height);
		}

		GLTexture2D::~GLTexture2D() {
			glDeleteTextures(1, &m_id);
		}

		void GLTexture2D::set_param(TextureParameter param, TextureParamValue value) {
			glTextureParameteri(m_id, Texture::tex_param_lookup[static_cast<uint32_t>(param)], Texture::tex_param_value_lookup[static_cast<uint32_t>(value)]);
		}

		uint32_t GLTexture2D::id() {
			return m_id;
		}

		std::shared_ptr<Texture2D> Texture2D::create(int width, int height, const void* data, TexturePixelFormatSized fmt) {
			return std::make_shared<GLTexture2D>(width, height, data, fmt);
		}

		/**
		* Framebuffer
		*/

		uint32_t Framebuffer::texture_attachment_types[Framebuffer::NUMBER_OF_ATTACHMENT_TYPES] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT };

		GLFramebuffer::GLFramebuffer() {
			glCreateFramebuffers(1, &m_id);
		}

		GLFramebuffer::~GLFramebuffer() {
			glDeleteFramebuffers(1, &m_id);
		}

		void GLFramebuffer::attach(std::shared_ptr<Texture2D> tex, FramebufferTextureAttachment attachment_type) {
			glNamedFramebufferTexture(m_id, Framebuffer::texture_attachment_types[static_cast<uint32_t>(attachment_type)], tex->id(), 0);
		}

		bool GLFramebuffer::complete() {
			return glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		}

		uint32_t GLFramebuffer::id() {
			return m_id;
		}

		std::shared_ptr<Framebuffer> Framebuffer::create() {
			return std::make_shared<GLFramebuffer>();
		}
	}

}
