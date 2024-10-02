#pragma once

#include <GLCore.h>

class Sandbox2D : public GLCore::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(GLCore::Event& event) override;
private:
    GLCore::OrthographicCameraController m_CameraController;

    glm::vec4 m_QuadColor = { 0.584f, 0.0f, 1.0f, 1.0f };
    
    GLCore::Ref<GLCore::Texture2D> m_CheckerboardTexture;
    GLCore::Ref<GLCore::Texture2D> m_PlayerTexture;
    GLCore::Ref<GLCore::Texture2D> m_TilesetWater;
    GLCore::Ref<GLCore::Texture2D> m_TilesetGrass;

    uint32_t m_MapWidth;
    uint32_t m_MapHeight;
    std::unordered_map<char, GLCore::Ref<GLCore::SubTexture2D>> m_TextureMap;
};

