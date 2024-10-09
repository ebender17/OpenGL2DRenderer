#pragma once

#include <GLCore.h>

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
    GLCore::OrthographicCameraController m_CameraController;

    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_EBO;
    // TODO : Use Shader Library
    std::unique_ptr<GLCore::Shader> m_Shader;
};

