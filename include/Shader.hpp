#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
    std::string geometrySource;
};

class Shader
{
    public:
        Shader(const std::string& filepath);
        ~Shader();
        Shader& operator=(const Shader& shader);

        void Bind() const;
        void Unbind() const;

        //Set uniforms
        void SetUniform1i(const std::string& name, int value);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform3f(const std::string& name, float v0, float v1, float v2);
        void SetUniform3f(const std::string& name, glm::vec3 pos);     
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

        void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

    private:
        int GetUniformLocation(const std::string& name) const;
        uint32_t CompileShader(const uint32_t type, const std::string& source);
        uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
        ShaderProgramSource ParseShader(const std::string& filepath);

        uint32_t m_RendererId;
        std::string m_FilePath;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};