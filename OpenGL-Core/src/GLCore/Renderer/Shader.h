#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace GLCore {

    class Shader
    {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        GLuint CompileShader(GLenum type, const std::string& source);
    private:
        uint32_t m_RendererID;
    };

}

