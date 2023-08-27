#pragma once
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <vector>
#include <stdint.h>

struct VertexBufferElement
{
    uint32_t type;
    uint32_t count;
    uint32_t normalized;

    static uint32_t GetSizeOfType(uint32_t type);
};

class VertexBufferLayout
{
    public:
        VertexBufferLayout();
        ~VertexBufferLayout();

        template<typename T>
        void Push(uint32_t count);

        inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; } 
        inline uint32_t GetStride() const { return m_Stride; }

    private:
        std::vector<VertexBufferElement> m_Elements;
        uint32_t m_Stride;
};