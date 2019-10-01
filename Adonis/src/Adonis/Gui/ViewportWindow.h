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

			ViewportWindow(std::unique_ptr<render::Texture2D>&& texture, bool is_main);

			auto draw()->void;

			inline auto texture_id()->uint32_t { return m_texture->id(); };

			inline auto focused()->bool { return m_focused; };

			inline auto resized()->bool { return m_size_changed; };

			inline auto is_main()->bool { return m_is_main; };

			inline auto width()->int { return m_width; };
			inline auto height()->int { return m_height; };

		private:

			float m_width{ 0 };
			float m_height{ 0 };

			float m_last_width{ 0 };
			float m_last_height{ 0 };

			bool m_focused{ false };

			bool m_size_changed{ false };

			bool m_is_main{ false };

			std::unique_ptr<render::Texture2D> m_texture;

		};


	}


}

#endif