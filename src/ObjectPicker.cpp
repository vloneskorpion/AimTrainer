#include "ObjectPicker.hpp"
#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

ObjectPicker::ObjectPicker(WindowData& windowData) :
        m_Framebuffer{0}
    ,   m_ColorTexture{0}
    ,   m_DepthTexture{0}
{
    //Fix this
    const auto [width , height] = windowData.getWindowSize();

    // Create framebuffer
    GLCall(glGenFramebuffers(1, &m_Framebuffer));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));

    // Create color texture
    GLCall(glGenTextures(1, &m_ColorTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorTexture));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32I, 800, 600, 0, GL_RGBA_INTEGER, GL_INT, NULL);
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0));

    // Create depth texture
    GLCall(glGenTextures(1, &m_DepthTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthTexture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) not_eq GL_FRAMEBUFFER_COMPLETE)
    {
        LOG(ERROR) << "Framebuffer is not complete!";
        ASSERT(false);
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

ObjectPicker::~ObjectPicker()
{
    glDeleteFramebuffers(1, &m_Framebuffer);
    glDeleteTextures(1, &m_ColorTexture);
    glDeleteTextures(1, &m_DepthTexture);
}

void ObjectPicker::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void ObjectPicker::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ObjectPicker::PixelData ObjectPicker::getPixelData(float x, float y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    PixelData pixelData{};
    glReadPixels(x, y, 1, 1, GL_RGBA_INTEGER, GL_INT, &pixelData);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return pixelData;
}