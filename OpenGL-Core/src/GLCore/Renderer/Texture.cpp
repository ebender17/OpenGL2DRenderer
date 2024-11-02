#include "glpch.h"
#include "GLCore/Renderer/Texture.h"

#include "GLCore/Renderer/Renderer.h"
#include "Platform/OpenGL/Renderer/OpenGLTexture.h"

namespace GLCore {

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLTexture2D>(width, height);
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                GLCORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLTexture2D>(path);
        }

        GLCORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    void Texture2DLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
    {
        GLCORE_ASSERT(!Exists(name), "Texture already exists!");
        m_Textures[name] = texture;
    }

    void Texture2DLibrary::Add(const Ref<Texture2D>& texture)
    {
        auto& name = texture->GetName();
        Add(name, texture);
    }

    Ref<Texture2D> Texture2DLibrary::Load(const std::string& filepath)
    {
        return Ref<Texture2D>();
    }

    Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filepath)
    {
        return Ref<Texture2D>();
    }

    Ref<Texture2D> Texture2DLibrary::Get(const std::string& name)
    {
        return Ref<Texture2D>();
    }

    bool Texture2DLibrary::Exists(const std::string& name) const
    {
        return false;
    }

}