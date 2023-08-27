#pragma once

struct Mouse
{
    float x, y;
};

struct Time
{
    float lastFrameTime;
    float deltaTime;
};

class WindowData
{
    public:
        WindowData();
        ~WindowData() = default;

        inline Mouse& getMouseData() { return m_Mouse; }
        inline Time& getTimeData() { return m_Time; }

    private:
        Mouse m_Mouse;
        Time m_Time;
};