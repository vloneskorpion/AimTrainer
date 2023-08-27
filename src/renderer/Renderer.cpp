#include "renderer/Renderer.hpp"
#include "ErrorHandle.hpp"

#include <glad/glad.h>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawArrays(const VertexArray& vertexArray, const Shader& shader, uint32_t count) const
{
    shader.Bind();
    vertexArray.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}