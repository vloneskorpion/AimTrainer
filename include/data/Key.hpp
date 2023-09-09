#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

enum class KeyState {
    PRESSED,
    IDLE
};

enum class KeyType {
    SINGLE,
    CONTINUOUS
};

struct KeyInfo
{
    KeyType type;
    KeyState state;
};

// Remember to add proper key to WindowData::initializeKeyData() and ToString functions
enum SupportedKeys
{
    A = GLFW_KEY_A,
    D = GLFW_KEY_D,
    ESCAPE = GLFW_KEY_ESCAPE,
    M = GLFW_KEY_M,
    S = GLFW_KEY_S,
    W = GLFW_KEY_W
};

namespace key
{
    std::string supportedKeyToString(SupportedKeys key);
    std::string keyStateToString(KeyState state);
}