#pragma once

#include <string.h>

namespace Pumpkin {
    class Shader {
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        ~Shader();
        
        void Bind() const;
        void Unbind() const;
    private:
        uint32_t m_RendererID;
    };
}