#pragma once

namespace GLCore {

    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        bool SwapChainTarget = false; // if true render to screen
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual const FramebufferSpecification GetSpecification() const = 0;

        virtual const uint32_t GetColorAttachment() const = 0;
    public:
        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };

}

