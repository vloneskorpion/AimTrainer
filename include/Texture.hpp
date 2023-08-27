#pragma once

#include "renderer/Renderer.hpp"

class Texture
{
    public:
        Texture(const std::string& filepath);
        ~Texture();

        void Bind(uint32_t slot = 0) const;
        void Unbind() const;

        inline int32_t GetWidth() const { return m_Width; }
        inline int32_t GetHeight() const { return m_Height; }

        static void BindTexture(const uint32_t id, uint32_t slot = 0);

    private:
        uint32_t m_RendererId;
        std::string m_Filepath;
        unsigned char* m_LocalBuffer;
        int32_t m_Width, m_Height, m_BPP;
};