#pragma once

#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include "Adonis/Core.h"
#include "Adonis/ImGui/ImGuiImplRenderer.h"
#include "imgui.h"

namespace Adonis {

	namespace gui {


		class ADONIS_API ViewportWindow {
		public:

			ViewportWindow(float width, float height);

			auto draw(void* tex_id)->void;

			inline auto focused()->bool { return m_focused; };

		private:

			float m_width;
			float m_height;

			bool m_focused;

		};


	}


}

#endif