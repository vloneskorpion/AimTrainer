#pragma once

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>

class ImGuiWrapper
{
    public:
        ImGuiWrapper(std::shared_ptr<GLFWwindow> window);
        ~ImGuiWrapper();
    
        void NewFrame();
        void Render();
        void Destroy();
        void handleKey();

    private:
        std::shared_ptr<GLFWwindow> m_Window;
        ImGuiIO* m_ImGuiIo;
};