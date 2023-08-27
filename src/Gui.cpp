#include "Gui.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

Gui::Gui(std::shared_ptr<GLFWwindow> window) :
    m_Window{window}
{
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window.get(), true);
    ImGui::StyleColorsDark();
}

Gui::~Gui()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

void Gui::NewFrame()
{
    ImGui_ImplGlfwGL3_NewFrame();
}

void Gui::Render()
{
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}
