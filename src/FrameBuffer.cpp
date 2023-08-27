#include "FrameBuffer.hpp"
#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"
#include <glad/glad.h>

FrameBuffer::FrameBuffer()
{
    GLCall(glGenFramebuffers(1, &m_RendererId));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));  
    CreateAndAttachTexutre();
    GenerateRenderBuffer();
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG(ERROR) << "Framebuffer is not complete!";
    }
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));    
}

void FrameBuffer::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

uint32_t FrameBuffer::GetTexture() const
{
    return m_Texture;
}

void FrameBuffer::CreateAndAttachTexutre()
{
    GLCall(glGenTextures(1, &m_Texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0));
}

void FrameBuffer::GenerateRenderBuffer()
{
    GLCall(glGenRenderbuffers(1, &m_RenderBuffer));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer));
}