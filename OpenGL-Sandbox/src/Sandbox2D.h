#pragma once

#include <GLCore.h>

#include "2D/Player/PlayerController.h"
#include "2D/Camera/TargetCamera2DController.h"

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

    GLCore::Ref<GLCore::Texture2D> m_TilesetOutside;

    uint32_t m_MapWidth = 0;
    uint32_t m_MapHeight = 0;
    std::unordered_map<char, GLCore::Ref<GLCore::SubTexture2D>> m_TexCoordsMap;
};

