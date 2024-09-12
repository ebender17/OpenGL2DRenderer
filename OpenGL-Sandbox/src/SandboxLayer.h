#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class SandboxLayer : public GLCore::Layer
{
public:
    SandboxLayer();
    virtual ~SandboxLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(GLCore::Event& event) override;
    virtual void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnImGuiRender() override;
private:
    std::shared_ptr<GLCore::Shader> m_Shader;
    std::shared_ptr<GLCore::VertexArray> m_VertexArray;

    std::shared_ptr<GLCore::Shader> m_FlatColorShader;
    std::shared_ptr<GLCore::VertexArray> m_QuadVertexArray;

    GLCore::OrthographicCamera m_Camera;

    glm::vec3 m_QuadColor = { 0.0549, 0.0824, 0.227 };
};

