#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Camera.hpp"

class MousePicker
{
    using WindowPtr = std::shared_ptr<GLFWwindow>;

    public:
        MousePicker(WindowPtr window, Camera& camera);
        ~MousePicker();

        void Update();
        glm::vec3 calculateMouseRay();

        glm::vec3 toEyeCoords(glm::vec4 clipCoords, const glm::mat4& projectionMatrix);
        glm::vec3 toWorldCoords(glm::vec4 eyeCoords);
        glm::vec2 getNormalizedDeviceCoords(float mouseX, float mouseY, float width, float height);

    private:
        WindowPtr m_Window;
        Camera& m_Camera;
};