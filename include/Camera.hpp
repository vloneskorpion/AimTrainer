#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "data/WindowData.hpp"

namespace camera
{
    enum class Direction
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
}

class Camera
{
    using Direction = camera::Direction;

    public:
        Camera(WindowData& data, glm::vec3 position,
            float mouseSensitivity, float pitch = 0.0f, float yaw = -90.0f);
        ~Camera();
    
        void updateVectors();
        void processMouseMovement();
        void processKeyboardMovement(const Direction direction, const float deltaTime);

        glm::vec2 calculateMousePosOffsets();

        glm::mat4 getViewMatrix() const;

        inline void setMouseSensitivity(float mouseSensitivity) { m_MouseSensitivity = mouseSensitivity; }
        inline void disable() { m_Enabled = false; }
        inline float getFov() const { return m_Fov; }
        inline glm::vec3 getFront() const { return m_Front; }
        inline glm::vec3 getPosition () const { return m_Position; }

    private:
        WindowData& m_WindowData;

        float m_Pitch, m_Yaw;
        float m_Fov;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_Position;
        glm::vec3 m_WorldUp;

        float m_MovementSpeed;
        float m_MouseSensitivity;
        bool m_FirstMouseTick;
        double m_lastXMousePos, m_lastYMousePos;
        bool m_Enabled{true};
};