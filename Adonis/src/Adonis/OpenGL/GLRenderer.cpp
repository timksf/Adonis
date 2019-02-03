#pragma once
#include "pch.h"
#include "Adonis/Core.h"
#include "GLRenderer.h"


namespace Adonis {

	namespace render {

		GLRenderer::GLRenderer(const Color& clear_color) : RenderDevice(clear_color){
			ON_EVENT_BIND(PreRenderEvent,	GLRenderer);
			ON_EVENT_BIND(RenderEvent,		GLRenderer);
			ON_EVENT_BIND(UpdateEvent,		GLRenderer);
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

		void GLRenderer::on_PreRenderEvent(const event_ptr_t<PreRenderEvent>& ev) {
			this->clear();
		}

		void GLRenderer::on_RenderEvent(const event_ptr_t<RenderEvent>& e) {

		}

		void GLRenderer::attach()const {

		}

		void GLRenderer::detach()const {

		}

		void GLRenderer::on_UpdateEvent(const event_ptr_t<UpdateEvent>& e) {

		}

		/*
			SHADER
		*/


		void GLUtil::check(GLuint shader_id, const std::string& type) {
			int success;
			char infoLog[1024];
			if (type == "PROGRAM") {
				glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader_id, 1024, NULL, infoLog);
					AD_CORE_ERROR("Linking error in shader program: ");
					AD_CORE_ERROR(infoLog);
					AD_CORE_ERROR(" ------------------------------------------------------- ");
				}
			}
			else {
				glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader_id, 1024, NULL, infoLog);
					AD_CORE_ERROR("Compile error in shader of type: {0}", type);
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
			GLUtil::check(m_id, "Vertex");
		}

		GLVertexShader::~GLVertexShader() {
			glDeleteShader(m_id);
		}

		GLFragmentShader::GLFragmentShader(const std::string& code): FragmentShader(code) {
			const char* code_c = code.c_str();
			m_id = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(m_id, 1, &code_c, NULL);
			glCompileShader(m_id);
			GLUtil::check(m_id, "Fragment");
		}

		GLFragmentShader::~GLFragmentShader() {
			glDeleteShader(m_id);
		}

		std::shared_ptr<VertexShader> VertexShader::create(const std::string& code) {
			return std::make_shared<GLVertexShader>(code);
		}

		std::shared_ptr<FragmentShader> FragmentShader::create(const std::string& code) {
			return std::make_shared<GLFragmentShader>(code);
		}

	}

}
