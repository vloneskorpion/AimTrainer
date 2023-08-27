#pragma once

#include <stdint.h>

class FrameBuffer
{
    public:
        FrameBuffer();
        void Bind();
        void Unbind();
        uint32_t GetTexture() const;

    private:
        void CreateAndAttachTexutre();
        void GenerateRenderBuffer();
        uint32_t m_RendererId;
        uint32_t m_RenderBuffer;
        uint32_t m_Texture;
};