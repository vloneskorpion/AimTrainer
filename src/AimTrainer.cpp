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
#include "ObjectPicker.hpp"

#include "data/Matrices.hpp"

#include "handler/KeyHandler.hpp"

#include "utils/Utils.hpp"
#include "data/Key.hpp"

#include <vector>

namespace 
{
    struct GLFWWindowDeleter{
        void operator()(GLFWwindow* window) const {
            glfwDestroyWindow(window);
        }
    };

    glm::vec2 cameraFrontToScreenCoords(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::mat4 projectionMatrix, int screenWidth, int screenHeight, Camera& camera)
    {
        // Convert world space to clip space
        /*
            While transforming to camera space, coords will be always (0, 0, 0, 1) and it will be clipped as it is outside of the frustum.
            So we need to add the camera position to the front vector to get the correct clip space position.
        */
        glm::vec4 clipSpacePosition = projectionMatrix * camera.getViewMatrix() * glm::vec4(camera.getPosition() + camera.getFront(), 1.0f);

        // Convert to normalized device coordinates
        clipSpacePosition /= clipSpacePosition.w;

        // Convert clip space to screen space
        float xScreen = (clipSpacePosition.x + 1.0f) * 0.5f * screenWidth;
        float yScreen = (1.0f - clipSpacePosition.y) * 0.5f * screenHeight;

        return {xScreen, yScreen};
    }
}

AimTrainer::AimTrainer() :
        m_Window{nullptr}
    ,   m_ImGuiWrapper{nullptr}
    ,   m_Width{800}
    ,   m_Height{600}
    ,   m_WindowData{std::make_unique<WindowData>(m_Width, m_Height)}
    ,   m_Camera{*m_WindowData, glm::vec3{0.0f, 0.0f, 15.0f}, 0.05f}
    ,   m_Handlers{}
    ,   m_Shaders{}
{
    initOpenGl();
    //initGui();
    registerHandlers();
    createShaders();
}

AimTrainer::~AimTrainer()
{
    terminate();
}

void AimTrainer::run()
{
    m_Shaders.at("cube")->Bind();

    auto cubeRenderer = RenedererCreator::create<Shapes::Cube>(*m_Shaders.at("cube"));
    auto quadRenderer = RenedererCreator::create<Shapes::Quad>(*m_Shaders.at("quad"));
    auto crosshairRenderer = RenedererCreator::create<Shapes::Crosshair>(*m_Shaders.at("crosshair"));

    auto random = RandomGenerator{};

    std::vector<glm::vec3> positions;
    for(int i = 0; i < 10; i++)
    {
        auto pos = random.getRandomNumbers(3, -5.0f, 5.0f);
        positions.push_back({pos[0], pos[1], pos[2]});
    }

    auto& matrices = m_WindowData->getMatrices();

    ObjectPicker picker{*m_WindowData};

    ImGuiContext* ptr = nullptr;

    while (not glfwWindowShouldClose(m_Window.get()))
    {
        calculateDeltaTime();
        processInput();
        update();

        if(m_WindowData->getMenuEnabled())
        {
            ptr = ImGui::CreateContext();
            ImGui_ImplGlfwGL3_Init(m_Window.get(), true);
            ImGui::StyleColorsDark();
            ImGui::GetIO().MousePos = {m_WindowData->getMouseData().x, m_WindowData->getMouseData().y};
            ImGui::GetIO().Fonts->AddFontDefault();
            //ImGui::GetIO().Fonts->GetTexDataAsRGBA32();
            ImGui::GetIO().WantCaptureKeyboard = true;
            ImGui::GetIO().WantCaptureMouse = true;
            m_ImGuiWrapper->NewFrame();
        }

        matrices.model = glm::mat4(1.0f);
        matrices.view = m_Camera.getViewMatrix();
        matrices.projection = glm::perspective(glm::radians(m_Camera.getFov()), float(m_Width) / float(m_Height), 0.1f, 100.0f);

        // Picking phase
        picker.Bind();
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Shaders.at("picking")->Bind();
        for(size_t i = 0; i < positions.size(); i++)
        {
            const auto model = glm::translate(glm::mat4(1.0f), positions[i]);
            const auto mvp = matrices.projection * matrices.view * model;
            m_Shaders.at("picking")->SetUniformMatrix4f("u_MVP", mvp);
            m_Shaders.at("picking")->SetUniform1i("objectIndex", i + 1);
            m_Shaders.at("picking")->SetUniform1i("drawIndex", 0);
            cubeRenderer.setShader(std::ref(*m_Shaders.at("picking")));
            cubeRenderer.Draw();
        }
        picker.Unbind();

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        // Render phase
        {
            int clickedObject = 0;
            if(auto data = utils::getWindowData(m_Window.get()); data not_eq nullptr)
            {
                const auto crosshairScreenCoords = cameraFrontToScreenCoords(m_Camera.getPosition(), m_Camera.getFront(), matrices.projection, m_Width, m_Height, m_Camera);
                auto pixelData = picker.getPixelData(crosshairScreenCoords.x, m_Height - crosshairScreenCoords.y);
                clickedObject = pixelData.objectId;
                auto& mouse = data->getMouseData();

                if(clickedObject not_eq 0 && mouse.isLeftButtonPressed)
                {
                    clickedObject -= 1;
                    positions.erase(positions.begin() + clickedObject);
                    mouse.isLeftButtonPressed = false;
                }
            }

            m_Shaders.at("cube")->Bind();
            for(size_t i = 0; i < positions.size(); i++)
            {
                const auto model = glm::translate(glm::mat4(1.0f), positions[i]);
                const auto mvp = matrices.projection * matrices.view * model;
                m_Shaders.at("cube")->SetUniformMatrix4f("u_MVP", mvp);
                cubeRenderer.setShader(std::ref(*m_Shaders.at("cube")));
                cubeRenderer.Draw();
            }

            GLCall(glDisable(GL_DEPTH_TEST));
            m_Shaders.at("crosshair")->Bind();
            matrices.model = glm::mat4(1.0f);
            const auto mvp = matrices.model;
            m_Shaders.at("crosshair")->SetUniformMatrix4f("u_MVP", mvp);
            crosshairRenderer.Draw();
            GLCall(glEnable(GL_DEPTH_TEST));
        }

        //Render quad
        // {
        //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //     quadShader.Bind();
        //     glActiveTexture(GL_TEXTURE0);
        //     glBindTexture(GL_TEXTURE_2D, picker.getColorTexture());
        //     quadRenderer.Draw();
        // }

        if(m_WindowData->getMenuEnabled())
        {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        m_ImGuiWrapper->Render();
        ImGui::DestroyContext(ptr);
        ptr = nullptr;
        }

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

    if(m_Window.get() == NULL)
    {
        LOG(ERROR) << "Failed to create window";
        glfwTerminate();
    }

    setupCallbacks();
    glfwMakeContextCurrent(m_Window.get());
    glfwSetInputMode(m_Window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(m_Window.get(), GLFW_STICKY_MOUSE_BUTTONS, 1);

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
            if(auto data = utils::getWindowData(window); data not_eq nullptr)
            {
                auto& mouse = data->getMouseData();
                mouse.x = xPos;
                mouse.y = yPos;
                return;
            }
            LOG(ERROR) << "Failed to get window data";
        });
    
    glfwSetMouseButtonCallback(m_Window.get(), [](GLFWwindow* window, int button, int action, int mods)
        {
            if(auto data = utils::getWindowData(window); data not_eq nullptr)
            {
                auto& mouse = data->getMouseData().isLeftButtonPressed;
                if(button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS)
                {
                    mouse = true;
                }
                else if(button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_RELEASE)
                {
                    mouse = false;
                }
                return;
            }
            LOG(ERROR) << "Failed to get window data";
        });
}

void AimTrainer::registerHandlers()
{
    m_Handlers.push_back(std::make_unique<KeyHandler>(m_Window, m_Camera));
}

void AimTrainer::createShaders()
{
    m_Shaders.emplace("cube", std::make_unique<Shader>(FilePathManager::getPath("shaderTest.shader")));
    m_Shaders.emplace("crosshair", std::make_unique<Shader>(FilePathManager::getPath("crosshair.shader")));
    m_Shaders.emplace("picking", std::make_unique<Shader>(FilePathManager::getPath("picking.shader")));
    m_Shaders.emplace("quad", std::make_unique<Shader>(FilePathManager::getPath("quad.shader")));
}

void AimTrainer::initGui()
{
    m_ImGuiWrapper = std::make_unique<ImGuiWrapper>(m_Window);
}

void AimTrainer::calculateDeltaTime()
{
    if(auto data = utils::getWindowData(m_Window.get()); data not_eq nullptr)
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

