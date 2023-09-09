#pragma once

#include "Matrices.hpp"
#include "Key.hpp"
#include <map>

struct Mouse
{
    float x, y;
    bool isLeftButtonPressed;
    bool isRightButtonPressed;
};

struct Time
{
    float lastFrameTime;
    float deltaTime;
};

struct WindowSize
{
    float width, height;
};

class WindowData
{
    public:
        WindowData(float width, float height);
        ~WindowData() = default;

        inline Mouse& getMouseData() { return m_Mouse; }
        inline Time& getTimeData() { return m_Time; }
        inline WindowSize getWindowSize() const { return m_WindowSize; }
        inline Matrices& getMatrices() { return m_Matrices; }
        inline bool& getMenuEnabled() { return m_MenuEnabled; }
        inline std::map<SupportedKeys, KeyInfo>& getKeyData() { return m_KeyData; }

    private:
        WindowSize m_WindowSize;
        Mouse m_Mouse;
        Time m_Time;
        Matrices m_Matrices;

        // Keys
        std::map<SupportedKeys, KeyInfo> m_KeyData;
        void initializeKeyData();

        // Options
        bool m_MenuEnabled;
};