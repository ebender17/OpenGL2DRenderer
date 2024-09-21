#include "glpch.h"
#include "GLCore/Renderer/GraphicsContext.h"

#include "GLCore/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace GLCore {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}