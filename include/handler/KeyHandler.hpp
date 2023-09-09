#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <functional>

#include "handler/HandlerBase.hpp"
#include "data/Key.hpp"
#include "Camera.hpp"

class KeyHandler : public HandlerBase
{
    using WindowPtr = std::shared_ptr<GLFWwindow>;
    
    public:
        KeyHandler(WindowPtr window, Camera& camera);
        ~KeyHandler();

        void handle() override;
    
    private:
        bool isKeyState(const SupportedKeys key, const KeyState state) const;
        KeyType getKeyType(const SupportedKeys key) const;

        void setKeyState(const SupportedKeys key, const KeyState state);
        void setKeyStateToPressed(const SupportedKeys key);
        void setKeyStateToIdle(const SupportedKeys key);

        void processEvent(const SupportedKeys key, const std::function<void()> function);
        void processContinuousKey(const SupportedKeys key, const std::function<void()> function);
        void processSingleKey(const SupportedKeys key, const std::function<void()> function);

        WindowPtr m_Window;
        Camera& m_Camera;
};