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

	}

}
