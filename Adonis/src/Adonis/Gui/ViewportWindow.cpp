#include "pch.h"
#include "ViewportWindow.h"


namespace Adonis {

	namespace gui {

		ViewportWindow::ViewportWindow(float width, float height) :
			m_width(width),
			m_height(height) {
			m_focused = false;
		}



		void ViewportWindow::draw(void* tex_id) {

			if (!ImGui::Begin("Viewport")) {
				AD_CORE_ERROR("Failed to create viewport window!");
			}
			else {
				m_focused = ImGui::IsWindowFocused();

				auto x0 = ImGui::GetCursorScreenPos().x;
				auto y0 = ImGui::GetCursorScreenPos().y;

				m_width = ImGui::GetWindowSize().x;
				m_height = ImGui::GetWindowSize().y;

				auto lower_right = ImVec2(x0 + m_width, y0 + m_height);
				auto upper_left = ImVec2(x0, y0);
				ImGui::GetWindowDrawList()->AddImage(tex_id, upper_left, lower_right, { 0,1 }, { 1,0 });
				ImGui::End();
			}


		}


	}


}