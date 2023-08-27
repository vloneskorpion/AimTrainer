#include <glad/glad.h>
#include "easylogging/easylogging++.h"

#include "ErrorHandle.hpp"

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while(auto errorCode = glGetError())
    {
        auto error = std::string{};
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            // case GL_CONTEX_LOST: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            // case GL_TABLE_TOO_LARGE: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default: error = errorCode; break;
        }

        LOG(ERROR) << "[OpenGL Error] (" <<  error << ") " << function << " " << file << ":" << line;
        return false;
    }
    return true;
}