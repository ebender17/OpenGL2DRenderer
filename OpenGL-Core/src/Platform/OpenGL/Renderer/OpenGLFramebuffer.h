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

        virtual const FramebufferSpecification GetSpecification() const override { return m_Specification; }
        virtual const uint32_t GetColorAttachment() const override { return m_ColorAttachment; }
    private:
        FramebufferSpecification m_Specification;
        uint32_t m_RendererID;

        uint32_t m_ColorAttachment, m_DepthAttachment;
    };

}

