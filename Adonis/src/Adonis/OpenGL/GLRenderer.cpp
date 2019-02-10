#pragma once
#include "pch.h"
#include "Adonis/Core.h"
#include "GLRenderer.h"
#include "glm/gtc/type_ptr.hpp"


namespace Adonis {

	namespace render {

		GLRenderer::GLRenderer(const Color& clear_color) : RenderDevice(clear_color){
			ON_EVENT_BIND(PreRenderEvent,	GLRenderer);
			ON_EVENT_BIND(RenderEvent,		GLRenderer);
			ON_EVENT_BIND(UpdateEvent,		GLRenderer);
			m_renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
			m_glslversion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
			m_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		}

		GLRenderer::~GLRenderer() {

		}

		std::unique_ptr<RenderDevice> RenderDevice::create(const Color& clear_color) {
			return std::make_unique<GLRenderer>(clear_color);
		}

		void GLRenderer::clear() {
			glClearColor(clear_color.r(), clear_color.g(), clear_color.b(), clear_color.a());
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void GLRenderer::set_pipeline(std::shared_ptr<RenderPipeline> pipe) {
			glUseProgram(std::dynamic_pointer_cast<GLRenderPipeline>(pipe)->program_id());
		}

		void GLRenderer::on_PreRenderEvent(const event_ptr_t<PreRenderEvent>& ev) {
			this->clear();
		}

		void GLRenderer::on_RenderEvent(const event_ptr_t<RenderEvent>& e) {

		}

		void GLRenderer::on_UpdateEvent(const event_ptr_t<UpdateEvent>& e) {

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


		GLVertexShader::GLVertexShader(const std::string& code): VertexShader(code) {
			const char* code_c = code.c_str();
			m_id = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(m_id, 1, &code_c, NULL);
			glCompileShader(m_id);
			GLUtil::check_vert_shader(m_id);
		}

		GLVertexShader::~GLVertexShader() {
			glDeleteShader(m_id);
		}

		GLFragmentShader::GLFragmentShader(const std::string& code): FragmentShader(code) {
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

		GLRenderPipeline::GLRenderPipeline(std::unique_ptr<VertexShader> vertex_shader, std::unique_ptr<FragmentShader> frag_shader):
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

		GLuint GLRenderPipeline::program_id(){
			return m_program_id;
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

		GLPipelineParam::GLPipelineParam(const std::string& name, GLint location, GLuint program_id) :
			m_name(name),
			m_location(location),
			m_program_id(program_id)
		{

		}

		std::string GLPipelineParam::name(){
			return m_name;
		}

		GLint GLPipelineParam::location(){
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
			glUniformMatrix3fv(m_location, 3*3, false, glm::value_ptr(v[0]));
		}
		void GLPipelineParam::set_mat4f(glm::fmat4 v) {
			glUseProgram(m_program_id);
			glUniformMatrix3fv(m_location, 4*4, false, glm::value_ptr(v[0]));
		}

		/*
			Buffers
		*/

		GLVertexBuffer::GLVertexBuffer(int64_t size, const void* data) : VertexBuffer() {
			glCreateBuffers(1, &m_id);
			glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
		}

		GLVertexBuffer::~GLVertexBuffer() {
			glDeleteBuffers(1, &m_id);
		}

		GLIndexBuffer::GLIndexBuffer(int64_t size, const void* data) : IndexBuffer() {
			glCreateBuffers(1, &m_id);
			glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
		}

		GLIndexBuffer::~GLIndexBuffer() {
			glDeleteBuffers(1, &m_id);
		}

		std::unique_ptr<VertexBuffer> VertexBuffer::create(int64_t size, const void* data) {
			return std::make_unique<GLVertexBuffer>(size, data);
		}

		std::unique_ptr<IndexBuffer> IndexBuffer::create(int64_t size, const void* data) {
			return std::make_unique<GLIndexBuffer>(size, data);
		}

		/*
			Vertex Attributes
		*/

		GLVertexAttrib::GLVertexAttrib(GLuint binding, GLuint index, GLuint offset, GLsizei stride, GLenum type, GLuint size, GLboolean normalized) {
			_binding = binding;
			_index = index;
			_offset = offset;
			_stride = stride;
			_type = type;
			_size = size;
			_normalized = normalized;
		}

		/*
			Vertex Array/Buffer Description
		*/

		GLVertexArrayDesc::GLVertexArrayDesc(std::vector<std::unique_ptr<VertexAttrib>> attribs, GLuint baseoffset) :
			m_attribs(std::move(attribs)) {
			m_baseoffset = baseoffset;
		}

		std::vector<std::unique_ptr<VertexAttrib>>& GLVertexArrayDesc::attribs() {
			return m_attribs;
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

		bool GLVertexArray::add_buffer(GLuint id, std::shared_ptr<VertexArrayDesc> desc) {
			m_vbuffers[id] = desc;
			for (auto& attrib : desc->attribs()) {
				auto glattrib = dynamic_cast<GLVertexAttrib*>(attrib.get());
				GLuint bindingindex = static_cast<uint32_t>(m_vbuffers.size() - 1);
				glVertexArrayVertexBuffer(m_id, bindingindex, id, desc->baseoffset(), glattrib->_stride);
				glVertexArrayAttribFormat(m_id, glattrib->_index, glattrib->_size, glattrib->_type, glattrib->_normalized, glattrib->_offset);
				glVertexArrayAttribBinding(m_id, glattrib->_index, bindingindex);
			}
			return true;
		}

}

}
