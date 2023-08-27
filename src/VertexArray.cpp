#include <glad/glad.h>

#include "VertexArray.hpp"
#include "ErrorHandle.hpp"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererId));
    GLCall(glBindVertexArray(m_RendererId));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    vb.Bind();
    const auto& elements = layout.GetElements();
    auto offset = uint32_t{0};
    for(uint32_t i = 0; i < elements.size(); i++)
    {
        const auto& element = elements.at(i);
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, 
        element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}