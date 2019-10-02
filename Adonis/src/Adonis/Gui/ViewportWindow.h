#pragma once

#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include "Adonis/Core.h"
#include "Adonis/ImGui/ImGuiImplRenderer.h"
#include "Adonis/Rendering/Renderer.h"
#include "imgui.h"

namespace Adonis {

	namespace gui {

		enum class ADONIS_API ViewportWindowType : uint32_t {
			DEBUG = 0,
			MAIN
		};


		class ADONIS_API ViewportWindow {
		public:

			ViewportWindow(std::unique_ptr<render::Texture2D>&& texture, ViewportWindowType type);

			auto draw()->void;

			auto active()->bool;

			inline auto texture_id()->uint32_t { return m_texture->id(); };

			inline auto set_focus(bool focused)->void { m_focused = focused; };
				
			inline auto resized()->bool { return m_resized; };

			inline auto type()->ViewportWindowType { return m_type; };

			inline auto width()->int { return m_width; };
			inline auto height()->int { return m_height; };

		private:

			float m_width{ 0 };
			float m_height{ 0 };

			float m_last_width{ 0 };
			float m_last_height{ 0 };

			bool m_focused{ false };

			bool m_resized{ false };

			ViewportWindowType m_type;

			std::unique_ptr<render::Texture2D> m_texture;

			uint32_t m_id;

			static uint32_t s_window_count;

		};


	}


}

#endif