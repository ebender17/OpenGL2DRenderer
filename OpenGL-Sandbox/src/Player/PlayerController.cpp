#include "PlayerController.h"

using namespace GLCore;

PlayerController::PlayerController(const glm::vec3& position, const glm::vec2& spriteSize,
    float width, float height, const char* textureFilepath, GLCore::Ref<GameMap> gameMap)
    : GameObject2D(position, spriteSize, width, height), m_TextureFilepath(textureFilepath),
    m_GameMap(gameMap)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::LoadAssets()
{
    m_SpriteSheet = Texture2D::Create(m_TextureFilepath);
    m_Animator = CreateRef<AnimatorTopDown>();

    float timePerTile = 1.0f / m_Speed;
    float frameDurationWalk = timePerTile / 2.0f; // 2 steps in each 4 frames

    // IDLE DOWN
    SetupAnimation(m_IdleDown, false, 3, 1, 0.0f, 1);

    // WALK DOWN
    SetupAnimation(m_WalkDown, true, 3, 4, frameDurationWalk, 4);

    // IDLE UP
    SetupAnimation(m_IdleUp, false, 0, 1, 0.0f, 1);

    // WALK UP
    SetupAnimation(m_WalkUp, true, 0, 4, frameDurationWalk, 4);

    // IDLE LEFT
    SetupAnimation(m_IdleLeft, false, 2, 1, 0.0f, 1);

    // WALK LEFT
    SetupAnimation(m_WalkLeft, true, 2, 4, frameDurationWalk, 4);

    // IDLE RIGHT
    SetupAnimation(m_IdleRight, false, 1, 1, 0.0f, 1);

    // WALK RIGHT
    SetupAnimation(m_WalkRight, true, 1, 4, frameDurationWalk, 4);
}

void PlayerController::OnUpdate(GLCore::Timestep timestep)
{
    m_Animator->OnUpdate(timestep);

    // Handle turning delay
    if (m_PlayerState == PlayerState::Turning)
    {
        if (m_RemainingMoveDelay > 0.0f)
        {
            m_RemainingMoveDelay -= timestep;
            return;
        }

        m_PlayerState = PlayerState::Idle;
    }


    if (m_PlayerState == PlayerState::Idle)
        ProcessPlayerInput();
    else if (m_IsMoving)
        Move(timestep);
}

void PlayerController::OnRender()
{
    auto currentFrame = m_Animator->GetCurrentFrame()->SubTexture;
    Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, m_SpriteSize, currentFrame->GetTexture(), currentFrame->GetTexCoords());
}

void PlayerController::ProcessPlayerInput()
{
    if (m_IsMoving) return;

    Direction newDirection = m_CurrentDirection;
    glm::vec2 inputDirection = glm::vec2(0.0f);

    if (Input::IsKeyPressed(Key::A))
    {
        newDirection = Direction::West;
        inputDirection = { -1.0f, 0.0f };
    }
    else if (Input::IsKeyPressed(Key::D))
    {
        newDirection = Direction::East;
        inputDirection = { 1.0f, 0.0f };
    }
    else if (Input::IsKeyPressed(Key::W))
    {
        newDirection = Direction::North;
        inputDirection = { 0.0f, 1.0f };
    }
    else if (Input::IsKeyPressed(Key::S))
    {
        newDirection = Direction::South;
        inputDirection = { 0.0f, -1.0f };
    }

    if (inputDirection != glm::vec2(0.0f))
    {
        if (newDirection != m_CurrentDirection)
        {
            m_CurrentDirection = newDirection;
            m_RemainingMoveDelay = m_MoveDelay; // Reset delay
            SetActiveIdleAnimation();
            m_PlayerState = PlayerState::Turning;
            return;
        }

        m_IsMoving = true;
        m_Progress = 0.0f;
        m_StartPosition = m_Position;
        m_EndPosition = { m_Position.x + inputDirection.x, m_Position.y + inputDirection.y, m_Position.z };;

        SetActiveWalkAnimation();
        m_PlayerState = PlayerState::Walking;
    }
    else
    {
        SetActiveIdleAnimation();
        m_PlayerState = PlayerState::Idle;
    }
}

void PlayerController::Move(Timestep timestep)
{
    // Increment movement progress
    m_Progress += m_Speed * timestep;

    // Check if the target position is blocked
    if (m_GameMap->CheckCollision(m_EndPosition, m_Width, m_Height))
    {
        // If there's a collision, block movement but keep the walk animation playing
        m_Progress = 0.0f; // Reset progress
        m_IsMoving = false;
        m_PlayerState = PlayerState::Idle;

        // Keep the walk animation active to give visual feedback
        SetActiveWalkAnimation();

        return;
    }

    // If no collision, continue interpolation
    if (m_Progress >= 1.0f)
    {
        // Snap to target position and stop movement
        m_Position = m_EndPosition;
        m_Progress = 0.0f;
        m_IsMoving = false;
        m_PlayerState = PlayerState::Idle;
    }
    else
    {
        // Interpolate position between start and end
        m_Position = glm::mix(m_StartPosition, m_EndPosition, m_Progress);
    }
}

void PlayerController::SetupAnimation(const char* animationName, bool isLoop, unsigned int row, size_t frameCount, float frameDuration, unsigned int reserveFrameCount)
{
    Ref<AnimationTopDown> animation = CreateRef<AnimationTopDown>(animationName, isLoop, reserveFrameCount);
    bool frameIsSpriteSwap = frameDuration == 0.0f;
    for (int i = 0; i < frameCount; i++)
    {
        auto subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { i, row }, m_SpriteSizePixels);
        Ref<AnimationFrame> frame = CreateRef<AnimationFrame>(subTexture, frameIsSpriteSwap, frameDuration);
        animation->AddFrame(frame);
    }
    animation->SetAnimationStopCallback(std::bind(&PlayerController::OnAnimationEnd, this));
    m_Animator->AddAnimation(animation);
}

void PlayerController::OnAnimationEnd()
{
}

void PlayerController::SetActiveIdleAnimation()
{
    switch (m_CurrentDirection)
    {
    case Direction::South:
        m_Animator->SetActiveAnimation(m_IdleDown);
        break;
    case Direction::North:
        m_Animator->SetActiveAnimation(m_IdleUp);
        break;
    case Direction::West:
        m_Animator->SetActiveAnimation(m_IdleLeft);
        break;
    case Direction::East:
        m_Animator->SetActiveAnimation(m_IdleRight);
        break;
    }
}

void PlayerController::SetActiveWalkAnimation()
{
    switch (m_CurrentDirection)
    {
    case Direction::South:
        m_Animator->SetActiveAnimation(m_WalkDown);
        break;
    case Direction::North:
        m_Animator->SetActiveAnimation(m_WalkUp);
        break;
    case Direction::West:
        m_Animator->SetActiveAnimation(m_WalkLeft);
        break;
    case Direction::East:
        m_Animator->SetActiveAnimation(m_WalkRight);
        break;
    }
}
