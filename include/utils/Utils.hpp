#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "data/WindowData.hpp"

namespace utils
{
    WindowData* getWindowData(GLFWwindow* window);
}