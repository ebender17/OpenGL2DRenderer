#include "glpch.h"
#include "GLCore/Renderer/RendererAPI.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererAPI.h"

namespace GLCore {
    
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
        case RendererAPI::API::None:
            GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLRendererAPI>();
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
