#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"

#include <string>
#include <vector>
#include <memory>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct TextureData
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh 
{
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures);
        void Draw(Shader& shader) const;

        inline const VertexArray& GetVertexArray() const { return *m_VertexArray; }
        inline const std::vector<unsigned int>& GetIndicies() const { return m_Indices; }

    private:
        std::vector<Vertex>          m_Vertices;
        std::vector<unsigned int>    m_Indices;
        std::vector<TextureData>     m_Textures;
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

        void setupMesh();
};