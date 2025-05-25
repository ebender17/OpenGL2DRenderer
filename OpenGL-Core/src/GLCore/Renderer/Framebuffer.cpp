#include "glpch.h"
#include "Framebuffer.h"

#include "GLCore/Renderer/Renderer.h"
#include "Platform/OpenGL/Renderer/OpenGLFramebuffer.h"

namespace GLCore {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLFramebuffer>(spec);
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
        return Ref<Framebuffer>();
    }
}
