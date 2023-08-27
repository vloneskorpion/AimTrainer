#include "handler/MovementHandler.hpp"
#include "data/WindowData.hpp"
#include "easylogging/easylogging++.h"


MovementHandler::MovementHandler(WindowPtr window, Camera& camera)
    : m_Window(window)
    , m_Camera(camera)
{
}

void MovementHandler::handle()
{
    if(auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(m_Window.get())); data not_eq nullptr)
    {
        const auto deltaTime = data->getTimeData().deltaTime;
        handleKeys(deltaTime);
        return;
    }
    LOG(ERROR) << "Failed to get window data";
}

void MovementHandler::handleKeys(const float deltaTime)
{
    if(glfwGetKey(m_Window.get(), GLFW_KEY_W) == GLFW_PRESS)
        m_Camera.processKeyboardMovement(camera::Direction::FORWARD, deltaTime);
    if(glfwGetKey(m_Window.get(), GLFW_KEY_S) == GLFW_PRESS)
        m_Camera.processKeyboardMovement(camera::Direction::BACKWARD, deltaTime);
    if(glfwGetKey(m_Window.get(), GLFW_KEY_A) == GLFW_PRESS)
        m_Camera.processKeyboardMovement(camera::Direction::LEFT, deltaTime);
    if(glfwGetKey(m_Window.get(), GLFW_KEY_D) == GLFW_PRESS)
        m_Camera.processKeyboardMovement(camera::Direction::RIGHT, deltaTime);
}