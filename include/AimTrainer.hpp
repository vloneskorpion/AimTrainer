#pragma once

//OpenGl libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//C++ libs
#include <memory>
#include <stdint.h>
#include <vector>

//Project libs
#include "Gui.hpp"
#include "data/WindowData.hpp"
#include "Camera.hpp"
#include "handler/HandlerBase.hpp"

class AimTrainer
{
    using Handlers = std::vector<std::unique_ptr<HandlerBase>>;

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
        void initGui();
        void terminate();

        // Utilities
        std::shared_ptr<GLFWwindow> m_Window;
        std::unique_ptr<WindowData> m_WindowData;
        std::unique_ptr<Gui> m_Gui;
        Handlers m_Handlers;
        Camera m_Camera;

        // Configuration variables
        uint32_t m_Width, m_Height;
};