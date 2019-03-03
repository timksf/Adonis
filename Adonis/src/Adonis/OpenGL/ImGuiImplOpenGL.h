#pragma once

#include "Adonis/Core.h"
#include "Adonis/ImGui/ImGuiImplRenderer.h"

struct ImDrawData;

ADONIS_API bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = NULL);
ADONIS_API void     ImGui_ImplOpenGL3_Shutdown();
ADONIS_API void     ImGui_ImplOpenGL3_NewFrame();
ADONIS_API void     ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

ADONIS_API bool     ImGui_ImplOpenGL3_CreateFontsTexture();
ADONIS_API void     ImGui_ImplOpenGL3_DestroyFontsTexture();
ADONIS_API bool     ImGui_ImplOpenGL3_CreateDeviceObjects();
ADONIS_API void     ImGui_ImplOpenGL3_DestroyDeviceObjects();

