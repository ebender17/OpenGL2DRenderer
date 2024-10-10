#pragma once

#include <GLCore.h>

const uint16_t TILE_SIZE = 1; // TODO : pass this in somewhere

class PlayerTopDown
{
public:
    PlayerTopDown(const glm::vec3& position, const char* textureFilepath);
    ~PlayerTopDown();

    void LoadAssets();

    void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    void ProcessPlayerInput();
    void Move(GLCore::Timestep timestep);

    const glm::vec3& GetPosition() const { return m_Position; }
private:
    glm::vec3 m_Position;
    const char* m_TextureFilepath;

    GLCore::Ref<GLCore::Texture2D> m_SpriteSheet;
    GLCore::Ref<GLCore::SubTexture2D> m_Sprite;

    float m_Speed = 4.0f;
    glm::vec3 m_InitialPosition;
    glm::vec2 m_InputDirection = glm::vec2(0.0);
    bool m_IsMoving = false;
    float m_PercentMovedToNextTile = 0.0f;
};

