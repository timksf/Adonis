#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "glm/glm.hpp"

namespace Adonis {

	namespace render {

		struct Color {

			glm::fvec4 data;
			float& r() {
				return data[0];
			}
			float& g() {
				return data[1];
			}
			float& b() {
				return data[2];
			}
			float& a() {
				return data[3];
			}
		};

		class ADONIS_API RenderDevice : public EventListener {
		public:

			RenderDevice(const Color& clear_color) : clear_color(clear_color) {};

			static auto create(const Color& clear_color = { {0.0f, 0.0f, 0.0f, 0.0f} })->std::unique_ptr<RenderDevice>;

			virtual ~RenderDevice() {};

			ON_EVENT_DECL_P_V(PreRenderEvent);
			ON_EVENT_DECL_P_V(RenderEvent);
			ON_EVENT_DECL_P_V(UpdateEvent);

			virtual auto attach()const->void = 0;
			virtual auto detach()const->void = 0;
			virtual auto clear()->void = 0;


			Color clear_color;
		};
	}

}
