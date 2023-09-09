#pragma once

//OpenGl libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//C++ libs
#include <memory>
#include <stdint.h>
#include <vector>
#include <map>

//Project libs
#include "ImGuiWrapper.hpp"
#include "data/WindowData.hpp"
#include "Camera.hpp"
#include "handler/HandlerBase.hpp"

class Shader;

class AimTrainer
{
    using Handlers = std::vector<std::unique_ptr<HandlerBase>>;
    using Shaders = std::map<std::string, std::unique_ptr<Shader>>;

    public:
        AimTrainer();
        ~AimTrainer();

        void run();

    private:
        // Main loop
        void processInput();
        void update();
        void render();
        void calculateDeltaTime();

        // Initialization & destruction
        void initOpenGl();
        void setupCallbacks();
        void registerHandlers();
        void createShaders();
        void initGui();
        void terminate();

        // Utilities
        std::shared_ptr<GLFWwindow> m_Window;
        std::unique_ptr<WindowData> m_WindowData;
        std::unique_ptr<ImGuiWrapper> m_ImGuiWrapper;
        Handlers m_Handlers;
        Shaders m_Shaders;
        Camera m_Camera;

        // Configuration variables
        uint32_t m_Width, m_Height;
};