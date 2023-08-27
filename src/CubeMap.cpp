#include "CubeMap.hpp"

#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include "stb_image.h"

#include <glad/glad.h>

CubeMap::CubeMap(const std::vector<std::string>& faces) :
    m_RendererId{0},
    m_LocalBuffer{nullptr},
    m_Width{0},
    m_Height{0},
    m_BPP{0},
    m_Faces{faces}
{
    GLCall(glGenTextures(1, &m_RendererId));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));

    for(size_t i = 0; i < m_Faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(0);
        m_LocalBuffer = stbi_load(faces.at(i).c_str(), &m_Width, &m_Height, &m_BPP, 0);
        if(!m_LocalBuffer)
        {
            LOG(ERROR) << "Texture failed to load at path: " << faces.at(i);
            stbi_image_free(m_LocalBuffer);
            ASSERT(false);
        }

        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
        stbi_image_free(m_LocalBuffer);
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

CubeMap::~CubeMap()
{
    GLCall(glDeleteTextures(1, &m_RendererId));
}

void CubeMap::Bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));
}

void CubeMap::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}