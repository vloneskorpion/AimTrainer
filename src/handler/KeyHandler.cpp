#include "handler/KeyHandler.hpp"
#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include "data/WindowData.hpp"
#include "data/Key.hpp"
#include "utils/Utils.hpp"

namespace
{
    bool isKeyPressed(GLFWwindow* window, SupportedKeys key)
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool isKeyReleased(GLFWwindow* window, SupportedKeys key)
    {
        return glfwGetKey(window, key) == GLFW_RELEASE;
    }
}

KeyHandler::KeyHandler(WindowPtr window, Camera& camera)
    : 
        m_Window(window)
    ,   m_Camera(camera)
{
}

KeyHandler::~KeyHandler() = default;

void KeyHandler::handle()
{
    auto data = utils::getWindowData(m_Window.get());

    processEvent(SupportedKeys::ESCAPE, [this](){ glfwSetWindowShouldClose(m_Window.get(), true); });
    processEvent(SupportedKeys::M, [&data](){ data->getMenuEnabled() = not data->getMenuEnabled(); });
    processEvent(SupportedKeys::W, [&data, this](){ m_Camera.processKeyboardMovement(camera::Direction::FORWARD, data->getTimeData().deltaTime); });
    processEvent(SupportedKeys::S, [&data, this](){ m_Camera.processKeyboardMovement(camera::Direction::BACKWARD, data->getTimeData().deltaTime); });
    processEvent(SupportedKeys::A, [&data, this](){ m_Camera.processKeyboardMovement(camera::Direction::LEFT, data->getTimeData().deltaTime); });
    processEvent(SupportedKeys::D, [&data, this](){ m_Camera.processKeyboardMovement(camera::Direction::RIGHT, data->getTimeData().deltaTime); });
}

bool KeyHandler::isKeyState(const SupportedKeys key, const KeyState state) const
{
    return utils::getWindowData(m_Window.get())->getKeyData()[key].state == state;
}

void KeyHandler::setKeyState(const SupportedKeys key, const KeyState state)
{
    utils::getWindowData(m_Window.get())->getKeyData()[key].state = state;
}

void KeyHandler::setKeyStateToPressed(const SupportedKeys key)
{
    setKeyState(key, KeyState::PRESSED);
}

void KeyHandler::setKeyStateToIdle(const SupportedKeys key)
{
    setKeyState(key, KeyState::IDLE);
}

void KeyHandler::processEvent(const SupportedKeys key, const std::function<void()> pressAction)
{
    if (getKeyType(key) == KeyType::CONTINUOUS)
    {
        processContinuousKey(key, pressAction);
    }
    else if (getKeyType(key) == KeyType::SINGLE)
    {
        processSingleKey(key, pressAction);
    }
}

void KeyHandler::processContinuousKey(const SupportedKeys key, const std::function<void()> pressAction)
{
    if (isKeyPressed(m_Window.get(), key))
    {
        pressAction();
        setKeyStateToPressed(key);
    }
    else if (isKeyReleased(m_Window.get(), key))
    {
        setKeyStateToIdle(key);
    }
}

void KeyHandler::processSingleKey(const SupportedKeys key, const std::function<void()> pressAction)
{
    if (isKeyPressed(m_Window.get(), key) and isKeyState(key, KeyState::IDLE))
    {
        pressAction();
        setKeyStateToPressed(key);
    }
    else if (isKeyReleased(m_Window.get(), key))
    {
        setKeyStateToIdle(key);
    }
}

KeyType KeyHandler::getKeyType(const SupportedKeys key) const
{
    return utils::getWindowData(m_Window.get())->getKeyData()[key].type;
}
