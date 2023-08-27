#include "VertexBufferLayout.hpp"
#include "ErrorHandle.hpp"

#include <glad/glad.h>

uint32_t VertexBufferElement::GetSizeOfType(uint32_t type)
{
    switch(type)
    {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
    }
    ASSERT(false);
    return 0;
}

VertexBufferLayout::VertexBufferLayout() :
    m_Stride{0}
{

}

VertexBufferLayout::~VertexBufferLayout()
{

}

template<typename T>
void VertexBufferLayout::Push(uint32_t count)
{
    ASSERT(false);
}

template<>
void VertexBufferLayout::Push<float>(uint32_t count)
{
    m_Elements.push_back(VertexBufferElement{GL_FLOAT, count, GL_FALSE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
void VertexBufferLayout::Push<uint32_t>(uint32_t count)
{
    m_Elements.push_back(VertexBufferElement{GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
void VertexBufferLayout::Push<uint8_t>(uint32_t count)
{
    m_Elements.push_back(VertexBufferElement{GL_UNSIGNED_BYTE, count, GL_FALSE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}