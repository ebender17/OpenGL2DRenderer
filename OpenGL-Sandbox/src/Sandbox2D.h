#pragma once

#include <GLCore.h>

#include "2D/Player/PlayerController.h"
#include "2D/Camera/TargetCamera2DController.h"
#include "2D/Map/MapParser.h"
#include "2D/Debug/LineBox2D.h"

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
    
    GLCore::Ref<PlayerController> m_Player;
    GLCore::Ref<GameMap> m_GameMap;
    GLCore::Ref<LineBox2D> m_PlayerDebugBox;
};

