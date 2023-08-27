#pragma once

#include <GLFW/glfw3.h>
#include <memory>

#include "handler/HandlerBase.hpp"

class KeyHandler : public HandlerBase
{
    using WindowPtr = std::shared_ptr<GLFWwindow>;
    
    public:
        KeyHandler(WindowPtr window);
        ~KeyHandler();

        void handle() override;
    
    private:
        WindowPtr m_Window;
};