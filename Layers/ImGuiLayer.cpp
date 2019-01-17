//#include "pch.h"
//#include "ImGuiLayer.h"
//#include "Adonis/OpenGL/ImGuiImpl.h"
//#include "Adonis/OpenGL/ImGuiImplGLFW.h"
//#include "Adonis/Application.h"
//
//namespace Adonis {
//
//	ImGuiLayer::ImGuiLayer(Style style) : Layer(AD_NAME_FROM_TYPE(ImGuiLayer)), m_style(style) {
//		on_event<event::UpdateEvent>(std::bind(&ImGuiLayer::on_update, this, std::placeholders::_1));
//	}
//
//	void ImGuiLayer::on_update(const event::event_ptr_t<event::UpdateEvent>& event)const {
//
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//
//		bool x = true;
//
//		ImGui::ShowDemoWindow(&x);
//
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//	}
//
//	void ImGuiLayer::attach()const {
//		
//		ImGui::CreateContext();
//		
//		switch (m_style) {
//		case Style::Dark:
//			ImGui::StyleColorsDark();
//			break;
//		case Style::Light:
//			ImGui::StyleColorsLight();
//			break;
//		case Style::Classic:
//			ImGui::StyleColorsClassic();
//			break;
//		default: break;
//		}
//
//		ImGui_ImplGlfw_InitForOpenGL(Application::get()->consume_window()->consume_base_ptr(), true);
//		ImGui_ImplOpenGL3_Init("#version 150");
//
//
//	}
//
//	void ImGuiLayer::detach()const {
//
//		ImGui_ImplOpenGL3_Shutdown();
//		ImGui_ImplGlfw_Shutdown();
//		ImGui::DestroyContext();
//
//	}
//
//}