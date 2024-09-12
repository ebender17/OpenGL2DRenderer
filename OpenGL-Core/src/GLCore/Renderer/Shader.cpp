#include "glpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GLCore {

    Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSource, fragmentSource);
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}