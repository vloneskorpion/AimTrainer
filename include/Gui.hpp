#pragma once

#include <GLFW/glfw3.h>
#include <memory>

class Gui
{
    public:
        Gui(std::shared_ptr<GLFWwindow> window);
        ~Gui();
    
        void NewFrame();
        void Render();
        void Destroy();

    private:
        std::shared_ptr<GLFWwindow> m_Window;
};