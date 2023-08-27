#pragma once

#include <GLFW/glfw3.h>

#include "handler/HandlerBase.hpp"
#include "Camera.hpp"
#include <memory>

/* FWD */
namespace camera { enum class Direction; }

class MovementHandler : public HandlerBase
{
    using WindowPtr = std::shared_ptr<GLFWwindow>;
    
    public:
        MovementHandler(WindowPtr window, Camera& camera);
        ~MovementHandler() = default;

        void handle() override;
        void handleKeys(const float deltaTime);

    private:
        WindowPtr m_Window;
        Camera& m_Camera;
};