#include "glpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace GLCore {

    namespace Utils {

        static GLenum ImageFormatToGLDataFormat(ImageFormat format)
        {
            switch (format)
            {
                case ImageFormat::RGB8: return GL_RGB;
                case ImageFormat::RGBA8: return GL_RGBA;
            }

            GLCORE_ASSERT(false, "ImageFormat not available as a GLDataFormat!");
            return 0;
        }

        static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
        {
            switch (format)
            {
                case ImageFormat::RGB8: return GL_RGB8;
                case ImageFormat::RGBA8: return GL_RGBA8;
            }

            GLCORE_ASSERT(false, "ImageFormat not available as a GLInternalFormat!");
            return 0;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height)
    {
        // TODO : Support for other formats
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        GLCORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        switch (channels)
        {
            case 3:
                m_InternalFormat = Utils::ImageFormatToGLInternalFormat(ImageFormat::RGB8);
                m_DataFormat = Utils::ImageFormatToGLDataFormat(ImageFormat::RGB8);
                break;
            case 4:
                m_InternalFormat = Utils::ImageFormatToGLInternalFormat(ImageFormat::RGBA8);
                m_DataFormat = Utils::ImageFormatToGLDataFormat(ImageFormat::RGBA8);
                break;
        }

        GLCORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        // TODO : Support for other formats
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        GLCORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

}
