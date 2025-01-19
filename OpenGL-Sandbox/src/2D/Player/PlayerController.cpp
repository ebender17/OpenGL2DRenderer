#include "PlayerController.h"

using namespace GLCore;

PlayerController::PlayerController(const glm::vec3& position, const char* textureFilepath)
    : GameObject2D(position), m_TextureFilepath(textureFilepath)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::LoadAssets()
{
    m_SpriteSheet = Texture2D::Create(m_TextureFilepath);
    float invTextureWidth = 1.0f / m_SpriteSheet->GetWidth();
    float invTextureHeight = 1.0f / m_SpriteSheet->GetHeight();
    m_Width = m_SpriteSize.x * invTextureWidth; // TODO : check these are correct
    m_Height = m_SpriteSize.y * invTextureHeight;
    m_Animator = CreateRef<AnimatorTopDown>();

    // IDLE DOWN
    SetupAnimation(m_IdleDown, false, 3, 1, 0.1f, 1);

    // WALK DOWN
    SetupAnimation(m_WalkDown, true, 3, 4, 0.15f, 4);

    // IDLE UP
    SetupAnimation(m_IdleUp, false, 0, 1, 0.1f, 1);

    // WALK UP
    SetupAnimation(m_WalkUp, true, 0, 4, 0.15f, 4);

    // IDLE LEFT
    SetupAnimation(m_IdleLeft, false, 2, 1, 0.1f, 1);

    // WALK LEFT
    SetupAnimation(m_WalkLeft, true, 2, 4, 0.15f, 4);

    // IDLE RIGHT
    SetupAnimation(m_IdleRight, false, 1, 1, 0.1f, 1);

    // WALK RIGHT
    SetupAnimation(m_WalkRight, true, 1, 4, 0.15f, 4);
}

void PlayerController::OnUpdate(GLCore::Timestep timestep)
{
    m_Animator->OnUpdate(timestep);

    if (m_PlayerState == PlayerState::Turning)
        return;
    else if (m_PlayerState == PlayerState::Idle)
        ProcessPlayerInput();
    else if (m_InputDirection != glm::vec2(0.0f))
    {
        SetActiveWalkAnimation();
        Move(timestep);
    }
}

void PlayerController::OnRender()
{
    auto currentFrame = m_Animator->GetCurrentFrame()->SubTexture;
    Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.5f }, currentFrame->GetTexture(), currentFrame->GetTexCoords());
}

void PlayerController::ProcessPlayerInput()
{
    Direction newDirection = Direction::Down;
    m_InputDirection = glm::vec2(0.0f);

    if (m_InputDirection.y == 0 && Input::IsKeyPressed(Key::A))
    {
        newDirection = Direction::Left;
        m_InputDirection = { -1.0f, 0.0f };
    }
    if (m_InputDirection.y == 0 && Input::IsKeyPressed(Key::D))
    {
        newDirection = Direction::Right;
        m_InputDirection = { 1.0f, 0.0f };
    }
    if (m_InputDirection.x == 0 && Input::IsKeyPressed(Key::W))
    {
        newDirection = Direction::Up;
        m_InputDirection = { 0.0f, 1.0f };
    }
    if (m_InputDirection.x == 0 && Input::IsKeyPressed(Key::S))
    {
        newDirection = Direction::Down;
        m_InputDirection = { 0.0f, -1.0f };
    }

    if (m_InputDirection != glm::vec2(0.0f))
    {
        if (newDirection != m_Direction)
        {
            m_Direction = newDirection;
            SetActiveIdleAnimation();
            m_PlayerState = PlayerState::Turning;
        }
        else
        {
            m_PlayerState = PlayerState::Walking;
        }
    }
    else
    {
        SetActiveIdleAnimation();
    }
}

void PlayerController::Move(Timestep timestep)
{
    m_PercentMovedToNextTile += m_Speed * timestep;
    if (m_PercentMovedToNextTile >= 1.0)
    {
        m_Position.x = m_Position.x + m_InputDirection.x;
        m_Position.y = m_Position.y + m_InputDirection.y;
        m_PercentMovedToNextTile = 0.0f;
        m_PlayerState = PlayerState::Idle;
    }
    else
    {
        m_Position.x = m_Position.x + m_InputDirection.x * m_PercentMovedToNextTile;
        m_Position.y = m_Position.y + m_InputDirection.y * m_PercentMovedToNextTile;
    }
}

void PlayerController::SetupAnimation(const char* animationName, bool isLoop, unsigned int row, size_t frameCount, float frameDuration, unsigned int reserveFrameCount)
{
    Ref<AnimationTopDown> animation = CreateRef<AnimationTopDown>(animationName, isLoop, reserveFrameCount);
    for (int i = 0; i < frameCount; i++)
    {
        auto subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { i, row }, m_SpriteSize);
        Ref<AnimationFrame> frame = CreateRef<AnimationFrame>(subTexture, frameDuration);
        animation->AddFrame(frame);
    }
    animation->SetAnimationStopCallback(std::bind(&PlayerController::OnAnimationEnd, this)); // TODO : can pass in unique callbacks if needed
    m_Animator->AddAnimation(animation);
}

void PlayerController::OnAnimationEnd()
{
    if (m_PlayerState == PlayerState::Turning)
        m_PlayerState = PlayerState::Idle;
}

void PlayerController::SetActiveIdleAnimation()
{
    switch (m_Direction)
    {
    case Direction::Down:
        m_Animator->SetActiveAnimation(m_IdleDown);
        break;
    case Direction::Up:
        m_Animator->SetActiveAnimation(m_IdleUp);
        break;
    case Direction::Left:
        m_Animator->SetActiveAnimation(m_IdleLeft);
        break;
    case Direction::Right:
        m_Animator->SetActiveAnimation(m_IdleRight);
        break;
    }
}

void PlayerController::SetActiveWalkAnimation()
{
    switch (m_Direction)
    {
    case Direction::Down:
        m_Animator->SetActiveAnimation(m_WalkDown);
        break;
    case Direction::Up:
        m_Animator->SetActiveAnimation(m_WalkUp);
        break;
    case Direction::Left:
        m_Animator->SetActiveAnimation(m_WalkLeft);
        break;
    case Direction::Right:
        m_Animator->SetActiveAnimation(m_WalkRight);
        break;
    }
}
