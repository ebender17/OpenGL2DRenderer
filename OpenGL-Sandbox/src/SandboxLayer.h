#pragma once

#include <GLCore.h>
#include "Camera/InputCamera2DController.h"

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
    GLCore::ShaderLibrary m_ShaderLibrary; // TODO : Move to Renderer
    GLCore::Ref<GLCore::VertexArray> m_TriVertexArray;
    GLCore::Ref<GLCore::VertexArray> m_QuadVertexArray;
    
    GLCore::Ref<GLCore::Shader> m_VertexPosShader;

    GLCore::Ref<GLCore::Texture2D> m_Texture;
    GLCore::Ref<GLCore::Texture2D> m_CharacterSprite;

    InputCamera2DController m_CameraController;

    glm::vec4 m_QuadColor = { 0.0549, 0.0824, 0.227, 1.0f };
};

