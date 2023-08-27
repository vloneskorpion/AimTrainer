#include "Model.hpp"
#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include "stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
    auto filename = std::string{path};
    filename = directory + '/' + filename;

    auto textureId = uint32_t{};
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    auto data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        auto format = GLenum{};

        switch (nrComponents)
        {
        case 1: format = GL_RED;  break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        default: LOG(WARNING) << "Coudldn't match nrComponents, setting default GL_RGBA"; break;
        }

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        LOG(ERROR) << "Texture failed to load at path: " << path;
        stbi_image_free(data);
    }
    return textureId;
}

Model::Model(const std::string& path)
{
    LoadModel(path);
}

void Model::Draw(Shader& shader) const
{
    for(const auto& mesh : m_Meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel(const std::string& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(ERROR) << importer.GetErrorString();
        return;
    }
    // retrieve the directory path of the filepath
    m_Directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    auto vertices = std::vector<Vertex>{};
    auto indices = std::vector<unsigned int>{};
    auto textures = std::vector<TextureData>{};

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first
            auto vertex = Vertex{};
            auto vector = glm::vec3{};
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                auto vec = glm::vec2{};
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texCoords = vec;
            }
            else
            {
                vertex.texCoords = glm::vec2(0.0f, 0.0f); 
            }
            vertices.push_back(vertex);
        }
        // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            auto face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }

        if(mesh->mMaterialIndex >= 0)
        {
            auto material = scene->mMaterials[mesh->mMaterialIndex];    
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
            auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        // return a mesh object created from the extracted mesh data
        return Mesh{vertices, indices, textures};
}


std::vector<TextureData> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    auto textures = std::vector<TextureData>{};

        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            auto str = aiString{};
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            auto skip = false;
            for(unsigned int j = 0; j < m_LoadedTextures.size(); j++)
            {
                if(std::strcmp(m_LoadedTextures[j].path.c_str(), str.C_Str()) == 0)
                {
                    textures.push_back(m_LoadedTextures[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                auto texture = TextureData{};
                texture.id = TextureFromFile(str.C_Str(), this->m_Directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                m_LoadedTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
    return textures;
}