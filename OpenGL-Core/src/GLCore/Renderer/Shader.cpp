#include "glpch.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace GLCore {

    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        GLuint program = glCreateProgram();

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            LOG_ERROR("{0}", infoLog.data());
            GLCORE_ASSERT(false, "Shader link failure!");
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        m_RendererID = program;
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    GLuint Shader::CompileShader(GLenum type, const std::string& source)
    {
        GLuint shader = glCreateShader(type);
        const GLchar* sourceStr = source.c_str();
        glShaderSource(shader, 1, &sourceStr, 0);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);
            LOG_ERROR("{0}", infoLog.data());
            GLCORE_ASSERT(false, "Shader compilation failure!");
        }
        return shader;
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        // TODO : cache uniform locations & lookup
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

}