#include "MousePicker.hpp"
#include "easylogging/easylogging++.h"
#include "utils/Utils.hpp"

MousePicker::MousePicker(WindowPtr window, Camera& camera) :
        m_Window{window}
    ,   m_Camera{camera}
{
}

MousePicker::~MousePicker()
{
}

void MousePicker::Update()
{
}

glm::vec3 MousePicker::calculateMouseRay()
{
    auto data = utils::getWindowData(m_Window.get());
    if (not data) {
        LOG(ERROR) << "Failed to get window data";
        return glm::vec3(0.0f);
    }

    const auto mouseX = data->getMouseData().x;
    const auto mouseY = data->getMouseData().y;
    const auto [width, height] = data->getWindowSize();
    const auto projectionMatrix = data->getMatrices().projection;

    glm::vec2 normalizedCoords = getNormalizedDeviceCoords(mouseX, mouseY, width, height);
    glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
    glm::vec4 eyeCoords = glm::vec4(toEyeCoords(clipCoords, projectionMatrix), 0.0f);
    return glm::normalize(toWorldCoords(eyeCoords));
}

glm::vec2 MousePicker::getNormalizedDeviceCoords(float mouseX, float mouseY, float width, float height)
{
    const auto x = (2.0f * mouseX) / width - 1.0f;
    const auto y = (2.0f * mouseY) / height - 1.0f;
    return glm::vec2(x, -y);
}

glm::vec3 MousePicker::toEyeCoords(glm::vec4 clipCoords, const glm::mat4& projectionMatrix)
{
    const auto eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
    return glm::vec3(eyeCoords.x, eyeCoords.y, -1.0f);
}

glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords)
{
    const auto worldCoords = glm::inverse(m_Camera.getViewMatrix()) * eyeCoords;
    return glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z);
}