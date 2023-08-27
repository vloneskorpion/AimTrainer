#include "handler/KeyHandler.hpp"

KeyHandler::KeyHandler(WindowPtr window)
    : m_Window(window)
{
}

KeyHandler::~KeyHandler() = default;

void KeyHandler::handle()
{
    if (glfwGetKey(m_Window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window.get(), true);
}