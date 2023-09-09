#include "data/Key.hpp"

namespace key
{
    std::string supportedKeyToString(SupportedKeys key) 
    {
        switch (key)
        {
            case A: return "A";
            case D: return "D";
            case ESCAPE: return "ESCAPE";
            case M: return "M";
            case S: return "S";
            case W: return "W";
            default: return "Unknown";
        }
    }

    std::string keyStateToString(KeyState state) 
    {
        switch (state) 
        {
            case KeyState::IDLE: return "IDLE";
            case KeyState::PRESSED: return "PRESSED";
            default: return "Unknown";
        }
    }
}
