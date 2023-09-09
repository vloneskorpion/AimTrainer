#include "Camera.hpp"

#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(WindowData& data, glm::vec3 position, float mouseSensitivity, float pitch, float yaw) :
    m_WindowData{data},
    m_Position{position},
    m_MouseSensitivity{mouseSensitivity},
    m_MovementSpeed{2.5f},
    m_Pitch{pitch},
    m_Yaw{yaw},
    m_FirstMouseTick{true},
    m_Fov{45.0f}
{
    updateVectors();
}

Camera::~Camera() = default;

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::updateVectors()
{
    auto front = glm::vec3{};
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    
    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, /* WorldUp */ glm::vec3{0.0f, 1.0f, 0.0f}));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::processMouseMovement()
{
    if(not m_Enabled)
    {
        return;
    }
    auto offsetVec = calculateMousePosOffsets();
    
    m_Yaw += offsetVec.x * m_MouseSensitivity;
    m_Pitch -= offsetVec.y * m_MouseSensitivity;

    if(m_Pitch > 89.0f)          m_Pitch = 89.0f;
    else if(m_Pitch < -89.0f)    m_Pitch = -89.0f;

    updateVectors();
}

void Camera::processKeyboardMovement(const Direction direction, const float deltaTime)
{
    const auto velocity = m_MovementSpeed * deltaTime;
    switch(direction)
    {
        case Direction::FORWARD:
            m_Position += m_Front * velocity;
            break;
        case Direction::BACKWARD:
            m_Position -= m_Front * velocity;
            break;
        case Direction::LEFT:
            m_Position -= m_Right * velocity;
            break;
        case Direction::RIGHT:
            m_Position += m_Right * velocity;
            break;
        default:
            LOG(ERROR) << "Invalid direction";
            break;
    }
}

glm::vec2 Camera::calculateMousePosOffsets()
{
    const auto x = m_WindowData.getMouseData().x;
    const auto y = m_WindowData.getMouseData().y;
    
    if(m_FirstMouseTick)
    {
        m_lastXMousePos = x;
        m_lastYMousePos = y;
        m_FirstMouseTick = false;
    }

    const auto xOffset = x - m_lastXMousePos;
    const auto yOffset = y - m_lastYMousePos;

    m_lastXMousePos = x;
    m_lastYMousePos = y;

    return {xOffset, yOffset};
}