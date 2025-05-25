#pragma once

#include "GLCore/Renderer/Framebuffer.h"

namespace GLCore {

    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer() override;

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
        {
            GLCORE_ASSERT(index < m_ColorAttachments.size(), "Color Attachment does not exist!");
            return m_ColorAttachments[index];
        }
        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    private:
        FramebufferSpecification m_Specification;
        uint32_t m_RendererID;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };

}

