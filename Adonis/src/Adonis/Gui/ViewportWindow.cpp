#include "pch.h"
#include "ViewportWindow.h"
#include "Adonis/Eventsystem/EventManager.h"
#include "Adonis/Eventsystem/Events/Events.h"

namespace Adonis {

	namespace gui {

		uint32_t ViewportWindow::s_window_count = 0; 

		ViewportWindow::ViewportWindow(std::unique_ptr<render::Texture2D>&& texture, ViewportWindowType type) :
			m_type(type),
			m_texture(std::move(texture))
		{
			m_id = s_window_count++;
			m_focused = false;
			m_last_width = m_width;
			m_last_height = m_height;
		}


		void ViewportWindow::draw() {

			std::stringstream name;
			name << "Viewport" << m_id;

			if (!ImGui::Begin(name.str().c_str())) {
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
					m_resized = true;
				}
				else {
					m_resized = false;
				}


				m_last_width = m_width;
				m_last_height = m_height;

				auto lower_right = ImVec2(x0 + m_width, y0 + m_height);
				auto upper_left = ImVec2(x0, y0);

				ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<void*>(m_texture->id()), upper_left, lower_right, { 0,1 }, { 1,0 });
				ImGui::End();
			}


		}

		bool ViewportWindow::active() {

			return m_focused && (m_type == ViewportWindowType::MAIN);

		}


	}


}