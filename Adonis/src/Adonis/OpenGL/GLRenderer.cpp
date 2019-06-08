#pragma once
#include "pch.h"
#include "Adonis/Core.h"
#include "GLRenderer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Adonis/Math/Math.h"
#include "Adonis/RenderingSystem/Scene.h"

namespace Adonis {

	namespace render {

		namespace lookup_tables {

			AD_LOOKUP_TABLE_DEF_U32(drawmode, { GL_TRIANGLES, GL_POINTS, GL_LINES });
			AD_LOOKUP_TABLE_DEF_U32(buffer_bit, { GL_DYNAMIC_STORAGE_BIT, GL_MAP_READ_BIT, GL_MAP_WRITE_BIT });
			AD_LOOKUP_TABLE_DEF_U32(vertex_type, { GL_FLOAT, GL_HALF_FLOAT, GL_DOUBLE, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT });
			AD_LOOKUP_TABLE_DEF_U32(sized_pixel_format, { GL_R3_G3_B2, GL_RGB8, GL_RGBA4, GL_RGBA8, GL_DEPTH_COMPONENT16 });
			AD_LOOKUP_TABLE_DEF_U32(tex_param, { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER });
			AD_LOOKUP_TABLE_DEF_U32(pixel_format, { GL_RGB, GL_RGBA, GL_BGR, GL_BGRA });
			AD_LOOKUP_TABLE_DEF_U32(pixel_datatype, { GL_SHORT, GL_UNSIGNED_SHORT, GL_BYTE, GL_UNSIGNED_BYTE, GL_INT, GL_UNSIGNED_INT });
			AD_LOOKUP_TABLE_DEF_U32(tex_param_value, { GL_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE });
			AD_LOOKUP_TABLE_DEF_U32(texture_attachment_type, { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT });
			AD_LOOKUP_TABLE_DEF_U32(vertex_type_size, { 4u, 2u, 8u, 1u, 1u, 2u, 2u, 4u, 4u });
			AD_LOOKUP_TABLE_DEF_U32(drawmode_divisor, { 3u, 2u, 1u });

		}


		GLRenderer::GLRenderer(const Color& clear_color) : RenderDevice(clear_color) {
			AD_ON_EVENT_BIND(WindowResizeEvent, GLRenderer);
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

		void GLRenderer::draw_triangles(int offset, int count) {
			this->draw(DrawMethod::Classic, DrawMode::Triangles, offset, count);
		}

		void GLRenderer::draw(std::shared_ptr<Adonis::rendersystem::Scene> scene) {

			scene->draw_init();

			for (auto& mesh_spec : scene->mesh_specs()) {
				auto& mesh_group = scene->mesh_group(mesh_spec);
				mesh_group.activate();

				//Draw each mesh respectively
				for (uint32_t i = 0; i < mesh_group.number_of_models(); i++) {
					mesh_group.use_model(i);

					if (scene->type() == rendersystem::SceneType::Scene3D) {
						scene->pipe()->get_param("modelMatrix")->set_mat4f(mesh_group.active_model_matrix());
					}

					draw(mesh_spec.method(), mesh_spec.mode(), 0, mesh_group.active_model_prim_count());
				}

			}

		}

		void GLRenderer::draw(DrawMethod method, DrawMode mode, int offset, int count) {

			switch (method) {
				case DrawMethod::Classic:
					glDrawArrays(AD_LOOKUP_CORE(drawmode, mode), 0, count);
					break;
				case DrawMethod::Indexed:
					glDrawElements(AD_LOOKUP_CORE(drawmode, mode), count, GL_UNSIGNED_INT, 0);
					break;
			}

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

		std::shared_ptr<RenderPipeline> RenderPipeline::test_pipeline_2D() {
			return std::make_shared<GLRenderPipeline>(
				std::make_unique<GLVertexShader>(
#include "Adonis/OpenGL/Shaders/2DTest.vert"
					),
				std::make_unique<GLFragmentShader>(
#include "Adonis/OpenGL/Shaders/2DTest.frag"
					)
				);
		}

		std::shared_ptr<RenderPipeline> RenderPipeline::test_pipeline_3D() {
			return std::make_shared<GLRenderPipeline>(
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

		/*AD_LOOKUP_TABLE_DEF_U32(buffer_bit, { GL_DYNAMIC_STORAGE_BIT, GL_MAP_READ_BIT, GL_MAP_WRITE_BIT });*/
		
		/*
			Vertex buffer
		*/

		GLVertexBuffer::GLVertexBuffer(int64_t size, const void* data, BufferBit flags) : VertexBuffer() {
			glCreateBuffers(1, &m_id);
			std::stringstream ss;
			ss << std::hex << std::showbase << static_cast<uint32_t>(flags);
			ss.str(std::string());
			GLbitfield glflags = 0;
			for (uint32_t i = 0; i < AD_LOOKUP_TABLE_SIZE_NAME(buffer_bit); i++) {
				if ((i & static_cast<uint32_t>(flags)) == i) {
					glflags |= lookup_tables::buffer_bit_lookup[i];
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
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
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
			for (uint32_t i = 0; i < AD_LOOKUP_TABLE_SIZE_NAME(buffer_bit); i++) {
				if ((i & static_cast<uint32_t>(flags)) == 1) {
					glflags |= lookup_tables::buffer_bit_lookup[i];
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

		/*AD_LOOKUP_TABLE_DEF_U32(vertex_type, { GL_FLOAT, GL_HALF_FLOAT, GL_DOUBLE, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT });
		AD_LOOKUP_TABLE_DEF_U32(vertex_type_size, { 4u, 2u, 8u, 1u, 1u, 2u, 2u, 4u, 4u});*/

		GLVertexAttrib::GLVertexAttrib(GLuint index, GLuint offset, VertexType type, GLuint size, GLboolean normalized) {
			m_index = index;
			m_offset = offset;
			m_type = type;
			m_size = size;
			m_normalized = normalized;
			m_api_type = AD_LOOKUP_CORE(vertex_type, type);
		}

		std::unique_ptr<VertexAttrib> VertexAttrib::attrib3float(uint32_t index, uint32_t offset) {
			return std::move(VertexAttrib::create(index, offset, VertexType::FLOAT, 3));
		}

		std::unique_ptr<VertexAttrib> VertexAttrib::create(uint32_t index, uint32_t offset, VertexType type, uint32_t size) {
			bool normalized = (AD_ENUM_TO_UNDERLYING(type, VertexType) > 8);
			return std::make_unique<GLVertexAttrib>(index, offset, type, size, static_cast<GLboolean>(normalized));
		}

		/*
			Vertex Array/Buffer Description
		*/

		GLVertexArrayDesc::GLVertexArrayDesc(std::vector<std::unique_ptr<VertexAttrib>>&& attribs, GLuint baseoffset, GLsizei stride) :
			m_attribs(std::move(attribs)) {
			m_baseoffset = baseoffset;
			m_stride = stride;
			m_initialized = true;
		}

		GLVertexArrayDesc::GLVertexArrayDesc() {
		}

		std::vector<std::unique_ptr<VertexAttrib>>& GLVertexArrayDesc::attribs() {
			return m_attribs;
		}

		void GLVertexArrayDesc::add_attrib(std::unique_ptr<VertexAttrib>&& attrib) {
			m_attribs.push_back(std::move(attrib));
		}

		void GLVertexArrayDesc::add_attrib(VertexType type, uint32_t number, int custom_index, int custom_offset) {
			int index = custom_index < 0 ? static_cast<int>(m_attribs.size()) : custom_index;
			int offset = custom_offset < 0 ? 0 : custom_offset;

			//Calculate offset based on already added attributes
			if (offset == 0) {
				for (auto& attrib : m_attribs) {
					offset += attrib->size() * AD_LOOKUP_CORE(vertex_type_size, attrib->type());
				}
			}
			
			m_attribs.push_back(VertexAttrib::create(index, offset, type, number));
		}

		void GLVertexArrayDesc::force_init() {
			m_stride = 0;
			m_baseoffset = 0;

			for (auto& attr : m_attribs) {
				m_stride += attr->size() * AD_LOOKUP_CORE(vertex_type_size, attr->type());
			}

			m_initialized = true;
		}


		std::shared_ptr<VertexArrayDesc> VertexArrayDesc::create(std::vector<std::unique_ptr<VertexAttrib>>&& attribs, uint32_t baseoffset, uint32_t stride) {
			return std::make_shared<GLVertexArrayDesc>(std::move(attribs), baseoffset, stride);
		}

		std::shared_ptr<VertexArrayDesc> VertexArrayDesc::create_empty() {
			return std::make_shared<GLVertexArrayDesc>();
		}

		/*
			Vertex Array Object
		*/


		GLVertexArray::GLVertexArray(std::shared_ptr<VertexArrayDesc> desc): m_desc(desc){
			AD_CORE_ASSERT(desc->initialized(), "Vertex description not initialized");
			glCreateVertexArrays(1, &m_id);
			glVertexArrayVertexBuffer(m_id, 0, 0, m_desc->baseoffset(), m_desc->stride());
			for (uint32_t i = 0; i < m_desc->attribs().size(); i++) {
				this->add_attrib_to_underlying_obj(i);
			}
		}

		GLVertexArray::~GLVertexArray() {
			glDeleteVertexArrays(1, &m_id);
		}

		std::unique_ptr<VertexArray> VertexArray::create(std::shared_ptr<VertexArrayDesc> desc) {
			return std::make_unique<GLVertexArray>(desc);
		}

		void GLVertexArray::add_attrib_to_underlying_obj(uint32_t where) {
			auto& attrib = m_desc->attribs()[where];
			glEnableVertexArrayAttrib(m_id, attrib->index());
			glVertexArrayAttribFormat(m_id, attrib->index(), attrib->size(), static_cast<uint32_t>(attrib->api_type()), attrib->normalized(), attrib->offset());
			glVertexArrayAttribBinding(m_id, attrib->index(), m_current_bindingindex);
			//AD_CORE_INFO("Bindingindex: {0}, attribindex: {1}", m_current_bindingindex, attrib->index());
		}

		void GLVertexArray::set_vertex_buffer(GLuint vbo, GLuint bindingindex, int32_t custom_baseoffset, int32_t custom_stride) {
			int32_t baseoffset = (custom_baseoffset < 0) ? m_desc->baseoffset() : custom_baseoffset;
			int32_t stride = (custom_stride < 0) ? m_desc->stride() : custom_stride;
			glVertexArrayVertexBuffer(m_id, bindingindex, vbo, baseoffset, stride);
		}

		uint32_t GLVertexArray::add_desc(std::shared_ptr<VertexArrayDesc> desc, bool increase_bindingindex, bool overwrite_existing_attribs) {
			AD_CORE_ASSERT(desc->initialized(), "Vertex description not initialized");
			if (increase_bindingindex) {
				m_current_bindingindex++;
			}


			for (auto& attr : desc->attribs()) {
				if (attr == nullptr)
					continue;
				bool already_existing = false;
				for (auto it = m_desc->attribs().begin(); it != m_desc->attribs().end(); it++) {
					auto& mattr = *it;
					if (mattr->index() == attr->index()) {
						already_existing = true;
						if (overwrite_existing_attribs) {
							*it = std::move(attr);
							break;
						}
					}
				}
				if (increase_bindingindex || !already_existing && !overwrite_existing_attribs) {
					//AD_CORE_INFO("Attribute index: {0}, bindingindex: {1}", attr->index(), m_current_bindingindex);
					m_desc->attribs().push_back(std::move(attr));
					//Add to underlying VAO
					this->add_attrib_to_underlying_obj(static_cast<uint32_t>(m_desc->attribs().size()) - 1u);
				}
			}
			return m_current_bindingindex;
		}

		void GLVertexArray::set_index_buffer(uint32_t idx_buffer_id) {
			glVertexArrayElementBuffer(m_id, idx_buffer_id);
		}

		void GLVertexArray::use() {
			glBindVertexArray(m_id);
		}

		uint32_t GLVertexArray::id() {
			return m_id;
		}

		std::shared_ptr<VertexArrayDesc> GLVertexArray::desc() {
			return m_desc;
		}

		/**
		* Texture
		*/

		//AD_LOOKUP_TABLE_DEF_U32(sized_pixel_format, { GL_R3_G3_B2, GL_RGB8, GL_RGBA4, GL_RGBA8, GL_DEPTH_COMPONENT16 });
		//AD_LOOKUP_TABLE_DEF_U32(tex_param, { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER });
		//AD_LOOKUP_TABLE_DEF_U32(pixel_format, { GL_RGB, GL_RGBA, GL_BGR, GL_BGRA });
		//AD_LOOKUP_TABLE_DEF_U32(pixel_datatype, { GL_SHORT, GL_UNSIGNED_SHORT, GL_BYTE, GL_UNSIGNED_BYTE, GL_INT, GL_UNSIGNED_INT });
		//AD_LOOKUP_TABLE_DEF_U32(tex_param_value, { GL_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE });

		GLTexture2D::GLTexture2D(int width, int height, const void* data, TexturePixelFormatSized fmt) {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
			glTextureStorage2D(m_id, 1, AD_LOOKUP_CORE(sized_pixel_format, fmt), width, height);
		}

		GLTexture2D::~GLTexture2D() {
			glDeleteTextures(1, &m_id);
		}

		void GLTexture2D::set_param(TextureParameter param, TextureParamValue value) {
			glTextureParameteri(m_id, AD_LOOKUP_CORE(tex_param, param), AD_LOOKUP_CORE(tex_param_value, value));
		}

		uint32_t GLTexture2D::id() {
			return m_id;
		}

		std::unique_ptr<Texture2D> Texture2D::create(int width, int height, const void* data, TexturePixelFormatSized fmt) {
			return std::make_unique<GLTexture2D>(width, height, data, fmt);
		}

		/**
		* Framebuffer
		*/

		/*AD_LOOKUP_TABLE_DEF_U32(texture_attachment_type, { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT });*/

		GLFramebuffer::GLFramebuffer() : Framebuffer(max_color_attachments()) {
			glCreateFramebuffers(1, &m_id);
		}

		GLFramebuffer::~GLFramebuffer() {
			glDeleteFramebuffers(1, &m_id);
		}

		void GLFramebuffer::attach(uint32_t tex_id, FramebufferTextureAttachment attachment_type, int attachment_index) {
			if (attachment_type != FramebufferTextureAttachment::COLOR) {
				attachment_index = 0;
			}
			glNamedFramebufferTexture(m_id, AD_LOOKUP_CORE(texture_attachment_type, attachment_type) + attachment_index, tex_id, 0);
		}

		void GLFramebuffer::activate_color_attachment(int index) {
			glNamedFramebufferDrawBuffer(m_id, AD_LOOKUP_CORE(texture_attachment_type, FramebufferTextureAttachment::COLOR) + index);
		}

		bool GLFramebuffer::complete() {
			return glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		}

		uint32_t GLFramebuffer::id() {
			return m_id;
		}

		int GLFramebuffer::max_color_attachments() {
			static int lel;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &lel);
			return lel;
		}

		std::unique_ptr<Framebuffer> Framebuffer::create() {
			return std::make_unique<GLFramebuffer>();
		}
	}

}
