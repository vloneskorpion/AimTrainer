#include "data/WindowData.hpp"

WindowData::WindowData(float width, float height) :
    m_WindowSize{.width = width, .height = height},
    m_Mouse{.x = m_WindowSize.width / 2.0f, .y = m_WindowSize.height / 2.0f},
    m_Time{0.0f, 0.0f},
    m_Matrices{glm::mat4(0.0f), glm::mat4(0.0f), glm::mat4(0.0f)},
    m_MenuEnabled(false)
{
    initializeKeyData();
}

void WindowData::initializeKeyData()
{
    m_KeyData[SupportedKeys::A] =           {.type = KeyType::CONTINUOUS, .state = KeyState::IDLE};
    m_KeyData[SupportedKeys::D] =           {.type = KeyType::CONTINUOUS, .state = KeyState::IDLE};
    m_KeyData[SupportedKeys::ESCAPE] =      {.type = KeyType::SINGLE,     .state = KeyState::IDLE};
    m_KeyData[SupportedKeys::M] =           {.type = KeyType::SINGLE,     .state = KeyState::IDLE};
    m_KeyData[SupportedKeys::S] =           {.type = KeyType::CONTINUOUS, .state = KeyState::IDLE};
    m_KeyData[SupportedKeys::W] =           {.type = KeyType::CONTINUOUS, .state = KeyState::IDLE};
}