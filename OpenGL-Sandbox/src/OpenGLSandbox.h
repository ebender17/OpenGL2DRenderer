#pragma once

#pragma once

#include <GLCore.h>

#include "3D/Camera/FirstPersonCamera.h"
#include "3D/Lighting/Light.h"
#include "3D/Lighting/Material.h"

class OpenGLSandbox : public GLCore::Layer
{
public:
    OpenGLSandbox();
    virtual ~OpenGLSandbox() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(GLCore::Event& event) override;

private:
    void InitCamera();
    void GenerateTexture2D(const std::string& filepath, uint32_t* texture);

    bool OnWindowResized(GLCore::WindowResizeEvent& event);
private:
    uint32_t m_CubeVAO;
    uint32_t m_CubeVBO;
    uint32_t m_QuadVAO;
    uint32_t m_QuadVBO;

    uint32_t m_TileTexture;
    uint32_t m_MetalTexture;

    Material m_Material;

    // TODO : Use Shader Library
    std::unique_ptr<GLCore::Shader> m_Shader;
    std::unique_ptr<GLCore::Shader> m_FlatColorShader;

    //Camera
    std::unique_ptr<FirstPersonCamera> m_Camera;
};

