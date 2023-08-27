//OpenGl libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

//Configuration
#include "config.h"

//C++ libs

//Project libs
#include "AimTrainer.hpp"
#include "Logger.hpp"
#include "FilePathManager.hpp"

int main(int argc, char** argv)
{
    auto logger = Logger{FilePathManager::getPath("configuration.conf")};

    auto aimTrainer = AimTrainer{};
    aimTrainer.run();

    return 0;
}
