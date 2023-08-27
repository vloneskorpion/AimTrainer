#pragma once

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
    public:
        Renderer();
        ~Renderer();

        void Clear() const;
        void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
        void DrawArrays(const VertexArray& vertexArray, const Shader& shader, uint32_t count) const;
};