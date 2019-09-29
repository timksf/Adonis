#pragma once

#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include "Adonis/Core.h"
#include "Adonis/ImGui/ImGuiImplRenderer.h"
#include "Adonis/Rendering/Renderer.h"
#include "imgui.h"

namespace Adonis {

	namespace gui {


		class ADONIS_API ViewportWindow {
		public:

			ViewportWindow(std::unique_ptr<render::Texture2D>&& texture, float width, float height);

			auto draw()->void;

			inline auto focused()->bool { return m_focused; };

		private:

			float m_width{ 0 };
			float m_height{ 0 };

			float m_last_width{ 0 };
			float m_last_height{ 0 };

			bool m_focused{ false };

			bool m_size_changed{ false };

			std::unique_ptr<render::Texture2D> m_texture;

		};


	}


}

#endif