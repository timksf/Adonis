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

			auto attach()const->void override;
			auto detach()const->void override;
			auto clear()->void override;


		};

		class ADONIS_API GLUtil {
		public:
			static auto check(GLuint shader_id, const std::string& type)->void;

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

		class ADONIS_API GLRenderPipeline : RenderPipeline {
		public:

			GLRenderPipeline(GLVertexShader);

			auto program_id()->GLuint;
		private:
			GLuint m_program_id;
		};

	}

}
