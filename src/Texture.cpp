#include "Texture.hpp"
#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include "stb_image.h"

#include <glad/glad.h>

Texture::Texture(const std::string& filepath) :
    m_RendererId{0},
    m_Filepath{filepath},
    m_LocalBuffer{nullptr},
    m_Width{0},
    m_Height{0},
    m_BPP{0}
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    if(!m_LocalBuffer)
    {
        LOG(ERROR) << "Texture failed to load at path: " << m_Filepath;
        stbi_image_free(m_LocalBuffer);
        ASSERT(false);
    }

    GLCall(glGenTextures(1, &m_RendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    //Use GL_SRGB_ALPHA if you want textures to be linear gamma corrected
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    
    stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererId));
}

void Texture::Bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::BindTexture(const uint32_t id, uint32_t slot)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));
}