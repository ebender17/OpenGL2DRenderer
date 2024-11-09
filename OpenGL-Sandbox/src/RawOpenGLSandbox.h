#pragma once

#include <GLCore.h>

#include "3D/Camera/FirstPersonCamera.h"

class RawOpenGLSandbox : public GLCore::Layer
{
public:
    RawOpenGLSandbox();
    virtual ~RawOpenGLSandbox() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(GLCore::Event& event) override;

private:
    void InitCamera();

    bool OnWindowResized(GLCore::WindowResizeEvent& event);
private:
    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_EBO;
    uint32_t m_CheckerboardTexture;

    // TODO : Use Shader Library
    std::unique_ptr<GLCore::Shader> m_Shader;

    //Camera
    std::unique_ptr<FirstPersonCamera> m_Camera;

    // Light
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
};

