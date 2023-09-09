#include "renderer/RendererBase.hpp"
#include "ErrorHandle.hpp"
#include "VertexBufferLayout.hpp"
#include <glad/glad.h>
#include <numeric>
#include "easylogging/easylogging++.h"

namespace
{
    std::tuple<const std::vector<float>&, const std::vector<uint32_t>&> extractVertexData(const VertexData& data) 
    {
        return std::tie(data.vertices, data.vertexLayout);
    }

    const uint32_t calculateNumberOfElements(const RenderData& renderData)
    {
        const auto& [vertices, layout] = extractVertexData(renderData.vertexData);
        return vertices.size() / std::accumulate(std::begin(layout), std::end(layout), 0);
    }
}

RendererBase::RendererBase(std::reference_wrapper<Shader> shader, const RenderData& renderData) :
    m_Shader{shader},
    m_VertexArray{},
    m_VertexBuffer{nullptr},
    m_numberOfElements{calculateNumberOfElements(renderData)},
    m_Primivite{renderData.primivite}
{
    const auto& [vertices, layout] = extractVertexData(renderData.vertexData);
    
    m_VertexBuffer = std::move(std::make_unique<VertexBuffer>(vertices.data(), sizeof(float) * vertices.size()));
    auto vertexBufferLayout = VertexBufferLayout{};
    for(const auto& data : layout)
    {
        vertexBufferLayout.Push<float>(data);
    }

    m_VertexArray.AddBuffer(*m_VertexBuffer, vertexBufferLayout);
}

RendererBase::~RendererBase()
{
}

void RendererBase::Draw() const
{
    m_Shader.get().Bind();
    m_VertexArray.Bind();
    GLCall(glDrawArrays(m_Primivite, 0, m_numberOfElements));
}

void RendererBase::setShader(std::reference_wrapper<Shader> shader)
{
    m_Shader = shader;
}
