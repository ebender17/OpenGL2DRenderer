#pragma once

#include <GLCore.h>

#include "Player/PlayerController.h"
#include "Camera/TargetCamera2DController.h"
#include "Map/MapParser.h"
#include "Debug/LineBox2D.h"

class Sandbox2D : public GLCore::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(GLCore::Event& event) override;
private:
    GLCore::Ref<TargetCamera2DController> m_CameraController;

    glm::vec4 m_QuadColor = { 0.584f, 0.0f, 1.0f, 1.0f };
    glm::vec4 m_TextureErrorColor = { 0.992f, 0.242f, 0.707f, 1.0f };
    GLCore::Ref<GLCore::Shader> m_WaterReflectionShader;
    
    GLCore::Ref<PlayerController> m_Player;
    GLCore::Ref<GameMap> m_GameMap;
    GLCore::Ref<LineBox2D> m_PlayerDebugBox;
    GLCore::Ref<GLCore::Framebuffer> m_Framebuffer;
    uint32_t m_FullscreenVAO;
};

