#pragma once

#include <cstdint>
#include "data/WindowData.hpp"

class ObjectPicker
{
    public:
        struct PixelData
        {
            int objectId, _, __;
        };

        ObjectPicker(WindowData& windowData);
        ~ObjectPicker();

        void Bind() const;
        void Unbind() const;

        PixelData getPixelData(float x, float y);

        inline uint32_t getColorTexture() const { return m_ColorTexture; }
        
    private:
        uint32_t m_Framebuffer;
        uint32_t m_ColorTexture;
        uint32_t m_DepthTexture;
};