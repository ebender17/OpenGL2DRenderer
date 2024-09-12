#include "glpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GLCore {

    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
                GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
        case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexArray>();
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}