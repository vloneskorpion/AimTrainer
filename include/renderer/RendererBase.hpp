#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include <memory>

struct VertexData
{
    const std::vector<float> vertices;
    const std::vector<uint32_t> vertexLayout;
};

struct RenderData
{
    const VertexData vertexData;
    const uint32_t primivite;
};

class RendererBase
{
    public:
        void Draw() const;
        void setShader(Shader& shader);

        ~RendererBase();
        explicit RendererBase(Shader& shader, const RenderData& vertexData);

    private:
        Shader& m_Shader;
        VertexArray m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        const uint32_t m_numberOfElements;
        const uint32_t m_Primivite;
};