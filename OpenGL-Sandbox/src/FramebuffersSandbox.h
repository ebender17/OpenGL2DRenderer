#pragma once

#include <GLCore.h>

#include "3D/Camera/FirstPersonCamera.h"
#include "3D/Lighting/Material.h"

enum class PostProcesOptions
{
    Inversion = 0,
    Grayscale,
    Sharpen,
    Blur
};

class FramebuffersSandbox : public GLCore::Layer
{
public:
    FramebuffersSandbox();
    virtual ~FramebuffersSandbox() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(GLCore::Event& event) override;
private:
    void InitCamera();
    void GenerateTexture2D(const std::string& filepath, uint32_t* texture, uint32_t wrapOption);
    void GenerateTextureFramebuffer(uint32_t* texture);

    bool OnWindowResized(GLCore::WindowResizeEvent& event);

    void DrawScene();
private:
    uint32_t m_CubeVAO;
    uint32_t m_CubeVBO;
    uint32_t m_QuadVAO;
    uint32_t m_QuadVBO;
    uint32_t m_PlaneVAO;
    uint32_t m_PlaneVBO;
    uint32_t m_FBO;
    uint32_t m_RBO;

    uint32_t m_WoodenBoxTexture;
    uint32_t m_MetalTexture;
    uint32_t m_FramebufferTexture;

    Material m_Material;

    // TODO : Use Shader Library
    std::unique_ptr<GLCore::Shader> m_Shader;
    std::unique_ptr<GLCore::Shader> m_FullScreenQuadShader;

    //Camera
    std::unique_ptr<FirstPersonCamera> m_Camera;

    // Window
    float m_WindowWidth;
    float m_WindowHeight;

    PostProcesOptions m_PostProcessOption = PostProcesOptions::Inversion;
};

