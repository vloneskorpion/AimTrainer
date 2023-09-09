#include "utils/Utils.hpp"

namespace utils
{
    WindowData* getWindowData(GLFWwindow* window)
    {
        return static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    }
}