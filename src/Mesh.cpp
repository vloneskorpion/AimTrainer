#include "easylogging/easylogging++.h"
#include "Mesh.hpp"
#include "ErrorHandle.hpp"
#include "renderer/Renderer.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures) :
    m_Vertices{vertices}, 
    m_Indices{indices},
    m_Textures{textures},
    m_VertexArray{nullptr},
    m_IndexBuffer{nullptr}
{
    setupMesh();
}

void Mesh::Draw(Shader& shader) const
{

    auto diffuseNr = uint32_t{1};
    auto specularNr = uint32_t{1};

    for(uint32_t i = 0; i < m_Textures.size(); i++)
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + i));
        auto number = std::string{};
        auto name = m_Textures.at(i).type;
        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        auto fullName =  name + number; //std::string{"material."} +
        shader.SetUniform1i(fullName, i);
        GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i].id));
    }
    m_VertexArray->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, 0));
    m_VertexArray->Unbind();
}

void Mesh::setupMesh()
{
    
    m_VertexArray = std::make_unique<VertexArray>();
    auto vertexBuffer = VertexBuffer{m_Vertices.data(), m_Vertices.size() * sizeof(Vertex)};
    m_IndexBuffer = std::make_unique<IndexBuffer>(reinterpret_cast<const uint32_t*>(m_Indices.data()), m_Indices.size());

    auto vertexBufferLayout = VertexBufferLayout{};
    vertexBufferLayout.Push<float>(3);
    vertexBufferLayout.Push<float>(3);
    vertexBufferLayout.Push<float>(2);

    m_VertexArray->AddBuffer(vertexBuffer, vertexBufferLayout);
    m_VertexArray->Unbind();
}