#pragma once

#include <GLCore.h>

#include "../General/GameObject2D.h"
#include "../Animation/AnimatorTopDown.h"
#include "../Map/GameMap.h"

class PlayerController : public GameObject2D
{
public:
    enum class PlayerState
    {
        Idle = 0,
        Turning,
        Walking
    };

    enum class Direction
    {
        South = 0,
        North,
        West,
        East
    };
public:
    PlayerController(const glm::vec3& position, const glm::vec2& spriteSize,
        float width, float height, const char* textureFilepath, GLCore::Ref<GameMap> gameMap);
    ~PlayerController();

    void LoadAssets();

    void OnUpdate(GLCore::Timestep timestep);
    void OnRender();
    void OnRenderReflection();
private:
    void ProcessPlayerInput();
    void Move(GLCore::Timestep timestep);

    void SetupAnimation(const char* animationName, bool isLoop, unsigned int row, size_t frameCount, float frameDuration, unsigned int reserveFrameCount);
    void OnAnimationEnd();

    void SetActiveIdleAnimation();
    void SetActiveWalkAnimation();
private:
    const char* m_TextureFilepath;
    GLCore::Ref<GameMap> m_GameMap;
    GLCore::Ref<GLCore::Texture2D> m_SpriteSheet;
    GLCore::Ref<AnimatorTopDown> m_Animator;
    PlayerState m_PlayerState = PlayerState::Idle;

    float m_Speed = 5.0f;
    glm::vec2 m_InputDirection = glm::vec2(0.0);
    Direction m_CurrentDirection = Direction::South;
    bool m_IsMoving = false;
    float m_Progress = 0.0f; // Progress of the movement [0, 1]
    glm::vec3 m_StartPosition;
    glm::vec3 m_EndPosition;
    float m_MoveDelay = 0.2f;
    float m_RemainingMoveDelay = 0.0f;

    // Animation Names
    const char* c_IdleDown = "idle-down";
    const char* c_IdleUp = "idle-up";
    const char* c_IdleLeft = "idle-left";
    const char* c_IdleRight = "idle-right";

    const char* c_WalkDown = "walk-down";
    const char* c_WalkUp = "walk-up";
    const char* c_WalkLeft = "walk-left";
    const char* c_WalkRight = "walk-right";

    const float c_ReflectionZPosition = -0.06f;
};

