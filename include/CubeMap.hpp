#pragma once

#include <string>
#include <vector>

class CubeMap
{
    public:
        CubeMap(const std::vector<std::string>& faces);
        ~CubeMap();

        void Bind(uint32_t slot = 0) const;
        void Unbind() const;

        inline int32_t GetWidth() const { return m_Width; }
        inline int32_t GetHeight() const { return m_Height; }

    private:
        uint32_t m_RendererId;
        unsigned char* m_LocalBuffer;
        int32_t m_Width, m_Height, m_BPP;
        std::vector<std::string> m_Faces;
};