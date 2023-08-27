#pragma once

#include <stdint.h>

class VertexBuffer
{
    public:
        VertexBuffer(const void* data, long unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_RendererId;  
};