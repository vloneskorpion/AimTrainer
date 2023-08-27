#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Mesh.hpp"

#include <string>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
    public:
        Model(const std::string& path);
        void Draw(Shader& shader) const;
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<TextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

        inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; } 
        inline const std::vector<TextureData>& GetTextures() const { return m_LoadedTextures; }

        
    private:
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        std::vector<TextureData> m_LoadedTextures;
};