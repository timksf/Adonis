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
	}

}
