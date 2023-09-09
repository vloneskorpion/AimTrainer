#include "ImGuiWrapper.hpp"
#include <imgui/imgui_impl_glfw_gl3.h>

ImGuiWrapper::ImGuiWrapper(std::shared_ptr<GLFWwindow> window) :
        m_Window{window}
    ,   m_ImGuiIo{nullptr}
{
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window.get(), true);
    ImGui::StyleColorsDark();
    m_ImGuiIo = &ImGui::GetIO();
}

ImGuiWrapper::~ImGuiWrapper()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiWrapper::NewFrame()
{
    ImGui_ImplGlfwGL3_NewFrame();
}

void ImGuiWrapper::Render()
{
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::handleKey()
{
    if(ImGui::GetIO().KeysDown[ImGuiKey_Space])
    {
        m_ImGuiIo->WantCaptureMouse = false;
    }
}