#include "pch.h"
#include "Adonis/Core.h"
#include "Adonis/ImGui/ImGuiImplWindow.h"

struct GLFWwindow;

ADONIS_API bool     ImGui_ImplGlfw_InitForVulkan(GLFWwindow* window, bool install_callbacks);
ADONIS_API void     ImGui_ImplGlfw_Shutdown();
ADONIS_API bool     ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
ADONIS_API void     ImGui_ImplGlfw_NewFrame();

// InitXXX function with 'install_callbacks=true': install GLFW callbacks. They will call user's previously installed callbacks, if any.
// InitXXX function with 'install_callbacks=false': do not install GLFW callbacks. You will need to call them yourself from your own GLFW callbacks.
ADONIS_API void     ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
ADONIS_API void     ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
ADONIS_API void     ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
ADONIS_API void     ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);
