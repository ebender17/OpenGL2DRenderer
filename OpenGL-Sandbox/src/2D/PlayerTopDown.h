#pragma once

#include <GLCore.h>

#include "AnimatorTopDown.h"

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

    enum class Direction
    {
        Down = 0,
        Up,
        Left,
        Right
    };
private:
    glm::vec3 m_Position;
    const char* m_TextureFilepath;

    GLCore::Ref<GLCore::Texture2D> m_SpriteSheet;
    GLCore::Ref<AnimatorTopDown> m_Animator;

    float m_Speed = 5.0f;
    glm::vec3 m_InitialPosition;
    glm::vec2 m_InputDirection = glm::vec2(0.0);
    Direction m_Direction = Direction::Down;
    bool m_IsMoving = false;
    float m_PercentMovedToNextTile = 0.0f;

    GLCore::Ref<AnimationTopDown> idleDownAnim;
    GLCore::Ref<AnimationTopDown> idleUpAnim;
    GLCore::Ref<AnimationTopDown> idleLeftAnim;
    GLCore::Ref<AnimationTopDown> idleRightAnim;
    GLCore::Ref<AnimationTopDown> walkDownAnim;
    GLCore::Ref<AnimationTopDown> walkUpAnim;
    GLCore::Ref<AnimationTopDown> walkLeftAnim;
    GLCore::Ref<AnimationTopDown> walkRightAnim;
};

