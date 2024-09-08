#include "glpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GLCore {
    
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    std::unique_ptr<RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
        case RendererAPI::API::None:
            GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLRendererAPI>();
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
