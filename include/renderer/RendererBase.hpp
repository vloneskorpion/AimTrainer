#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include <memory>
#include <functional>

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
        void setShader(std::reference_wrapper<Shader> shader);

        ~RendererBase();
        explicit RendererBase(std::reference_wrapper<Shader>shader, const RenderData& vertexData);

    private:
        std::reference_wrapper<Shader> m_Shader;
        VertexArray m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        const uint32_t m_numberOfElements;
        const uint32_t m_Primivite;
};