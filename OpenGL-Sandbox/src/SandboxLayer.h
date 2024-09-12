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
    GLCore::Ref<GLCore::Shader> m_Shader;
    GLCore::Ref<GLCore::VertexArray> m_VertexArray;

    GLCore::Ref<GLCore::Shader> m_FlatColorShader;
    GLCore::Ref<GLCore::VertexArray> m_QuadVertexArray;
    
    GLCore::Ref<GLCore::Shader> m_TextureShader;
    GLCore::Ref<GLCore::Texture2D> m_Texture;

    GLCore::OrthographicCamera m_Camera;

    glm::vec3 m_QuadColor = { 0.0549, 0.0824, 0.227 };
};

