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
    unsigned int m_ScreenWidth;
    unsigned int m_ScreenHeight;
    uint32_t m_VAO;
    uint32_t m_VBO;
    uint32_t m_EBO;
    uint32_t m_CheckerboardTexture;
    // TODO : Use Shader Library
    std::unique_ptr<GLCore::Shader> m_Shader;

    //Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);

    // Light
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
};

