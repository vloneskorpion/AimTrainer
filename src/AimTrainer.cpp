#include "AimTrainer.hpp"

#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "FilePathManager.hpp"
#include "renderer/RendererCreator.hpp"
#include "RandomGenerator.hpp"
#include "Camera.hpp"

#include "handler/MovementHandler.hpp"
#include "handler/KeyHandler.hpp"

#include <vector>

namespace 
{
    struct GLFWWindowDeleter{
        void operator()(GLFWwindow* window) const {
            glfwDestroyWindow(window);
        }
    };
}

AimTrainer::AimTrainer() :
        m_Window{nullptr}
    ,   m_Gui{nullptr}
    ,   m_Width{800}
    ,   m_Height{600}
    ,   m_WindowData{std::make_unique<WindowData>()}
    ,   m_Camera{*m_WindowData, glm::vec3{0.0f, 0.0f, 15.0f}, 0.05f}
    ,   m_Handlers{}
{
    initOpenGl();
    initGui();
    registerHandlers();
}

AimTrainer::~AimTrainer()
{
    terminate();
}

void AimTrainer::run()
{
    // Basic shader setup
    auto shader = Shader{FilePathManager::getPath("shaderTest.shader")};
    shader.Bind();

    auto renderer = RenedererCreator::create<Shapes::Cube>(shader);

    auto random = RandomGenerator{};

    std::vector<glm::vec3> positions;

    for(int i = 0; i < 10; i++)
    {
        auto pos = random.getRandomNumbers(3, -5.0f, 5.0f);
        positions.push_back({pos[0], pos[1], pos[2]});
    }

    while (not glfwWindowShouldClose(m_Window.get()))
    {
        calculateDeltaTime();
        processInput();
        update();

        m_Gui->NewFrame();

        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        auto model = glm::mat4(1.0f);
        auto proj = glm::perspective(glm::radians(m_Camera.getFov()), 800.0f/600.0f, 0.1f, 100.0f);
        auto view = m_Camera.getViewMatrix();

        for(size_t i = 0; i < 10; i++)
        {
            model = glm::translate(glm::mat4(1.0f), positions[i]);
            const auto mvp = proj * view * model;
            shader.SetUniformMatrix4f("u_MVP", mvp);
            renderer.Draw();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        m_Gui->Render();

        GLCall(glfwSwapBuffers(m_Window.get()));
        GLCall(glfwPollEvents());
    }
    GLCall(glfwTerminate());
}

void AimTrainer::processInput()
{
    for(auto& handler : m_Handlers)
    {
        handler->handle();
    }
}

void AimTrainer::update()
{
    m_Camera.processMouseMovement();
}

void AimTrainer::render()
{
}

void AimTrainer::initOpenGl()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = std::shared_ptr<GLFWwindow>{glfwCreateWindow(m_Width, m_Height, "AimTrainer", NULL, NULL),
        GLFWWindowDeleter()};
    glfwSetWindowUserPointer(m_Window.get(), m_WindowData.get());
    setupCallbacks();

    if(m_Window.get() == NULL)
    {
        LOG(ERROR) << "Failed to create window";
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window.get());
    glfwSetInputMode(m_Window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG(ERROR) << "Failed to initialize GLAD";
        glfwTerminate();
    }

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));
    
}

void AimTrainer::setupCallbacks()
{
    glfwSetCursorPosCallback(m_Window.get(), [](GLFWwindow* window, double xPos, double yPos)
        {
            if(auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window)); data not_eq nullptr)
            {
                auto& mouse = data->getMouseData();
                mouse.x = xPos;
                mouse.y = yPos;
                return;
            }
            LOG(ERROR) << "Failed to get window data";
        });
}

void AimTrainer::registerHandlers()
{
    m_Handlers.push_back(std::make_unique<MovementHandler>(m_Window, m_Camera));
    m_Handlers.push_back(std::make_unique<KeyHandler>(m_Window));
}

void AimTrainer::initGui()
{
    m_Gui = std::make_unique<Gui>(m_Window);
}

void AimTrainer::calculateDeltaTime()
{
    if(auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(m_Window.get())); data not_eq nullptr)
    {
        auto currentFrame = glfwGetTime();
        auto& [lastFrame, deltaTime] = data->getTimeData();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        return;
    }
    LOG(ERROR) << "Failed to get window data";
}

void AimTrainer::terminate()
{
    glfwTerminate();
}

