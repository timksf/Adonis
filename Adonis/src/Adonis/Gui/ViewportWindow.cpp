#include "pch.h"
#include "ViewportWindow.h"
#include "Adonis/Eventsystem/EventManager.h"
#include "Adonis/Eventsystem/Events/Events.h"

namespace Adonis {

	namespace gui {

		ViewportWindow::ViewportWindow(std::unique_ptr<render::Texture2D>&& texture, bool is_main) :
			m_is_main(is_main),
			m_texture(std::move(texture))
		{
			m_focused = false;
			m_last_width = m_width;
			m_last_height = m_height;
		}


		void ViewportWindow::draw() {

			if (!ImGui::Begin("Viewport")) {
				AD_CORE_ERROR("Failed to create viewport window!");
			}
			else {
				m_focused = ImGui::IsWindowFocused();

				auto x0 = ImGui::GetCursorScreenPos().x;
				auto y0 = ImGui::GetCursorScreenPos().y;

				m_width = ImGui::GetWindowSize().x;
				m_height = ImGui::GetWindowSize().y;

				if ((m_width != m_last_width) || (m_height != m_last_height)) {
					m_texture->resize(m_width, m_height);
					m_size_changed = true;
				}
				else {
					m_size_changed = false;
				}


				m_last_width = m_width;
				m_last_height = m_height;

				auto lower_right = ImVec2(x0 + m_width, y0 + m_height);
				auto upper_left = ImVec2(x0, y0);

				ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<void*>(m_texture->id()), upper_left, lower_right, { 0,1 }, { 1,0 });
				ImGui::End();
			}


		}


	}


}