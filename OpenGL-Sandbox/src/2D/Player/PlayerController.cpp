#include "PlayerController.h"

using namespace GLCore;

PlayerController::PlayerController(const glm::vec3& position, const char* textureFilepath)
    : m_Position(position), m_TextureFilepath(textureFilepath), m_InitialPosition(position)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::LoadAssets()
{
    m_SpriteSheet = Texture2D::Create(m_TextureFilepath);
    m_Animator = CreateRef<AnimatorTopDown>();

    // IDLE DOWN
    SetupAnimation(m_IdleDown, false, {32, 48}, 3, 1, 0.0f); // TODO : check false works here

    // WALK DOWN
    SetupAnimation(m_WalkDown, true, {32, 48}, 3, 4, 0.15f);

    // IDLE UP
    SetupAnimation(m_IdleUp, false, {32, 48}, 0, 1, 0.0f); // TODO : check false works here

    // WALK UP
    SetupAnimation(m_WalkUp, true, {32, 48}, 0, 4, 0.15f);

    // IDLE LEFT
    SetupAnimation(m_IdleLeft, false, {32, 48}, 2, 1, 0.0f); // TODO : check false works here

    // WALK LEFT
    SetupAnimation(m_WalkLeft, true, {32, 48}, 2, 4, 0.15f);

    // IDLE RIGHT
    SetupAnimation(m_IdleRight, false, {32, 48}, 1, 1, 0.0f); // TODO : check false works here

    // WALK RIGHT
    SetupAnimation(m_WalkRight, true, {32, 48}, 1, 4, 0.15f);

    m_Direction = Direction::Down;
}

void PlayerController::OnUpdate(GLCore::Timestep timestep)
{
    m_Animator->OnUpdate(timestep);

    if (!isMoving)
    {
        ProcessPlayerInput();
    }
    else if (m_InputDirection != glm::vec2(0.0f))
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
    m_InputDirection = glm::vec2(0.0f);

    if (m_InputDirection.y == 0 && Input::IsKeyPressed(Key::A))
    {
        m_Direction = Direction::Left;
        m_InputDirection = { -1.0f, 0.0f };
    }
    if (m_InputDirection.y == 0 && Input::IsKeyPressed(Key::D))
    {
        m_Direction = Direction::Right;
        m_InputDirection = { 1.0f, 0.0f };
    }
    if (m_InputDirection.x == 0 && Input::IsKeyPressed(Key::W))
    {
        m_Direction = Direction::Up;
        m_InputDirection = { 0.0f, 1.0f };
    }
    if (m_InputDirection.x == 0 && Input::IsKeyPressed(Key::S))
    {
        m_Direction = Direction::Down;
        m_InputDirection = { 0.0f, -1.0f };
    }

    if (m_InputDirection != glm::vec2(0.0f))
    {
        m_InitialPosition = m_Position;
        isMoving = true;
    }
    else
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
}

void PlayerController::Move(Timestep timestep)
{
    m_PercentMovedToNextTile += m_Speed * timestep;
    if (m_PercentMovedToNextTile >= 1.0)
    {
        m_Position.x = m_InitialPosition.x + m_InputDirection.x;
        m_Position.y = m_InitialPosition.y + m_InputDirection.y;
        m_PercentMovedToNextTile = 0.0f;
        isMoving = false;
    }
    else
    {
        m_Position.x = m_InitialPosition.x + m_InputDirection.x * m_PercentMovedToNextTile;
        m_Position.y = m_InitialPosition.y + m_InputDirection.y * m_PercentMovedToNextTile;
    }
}

void PlayerController::SetupAnimation(const char* animationName, bool isLoop, const glm::vec2& spriteSize, unsigned int row, size_t frameCount, float frameDuration)
{
    Ref<AnimationTopDown> animation = CreateRef<AnimationTopDown>(animationName, isLoop);
    for (int i = 0; i < frameCount; i++)
    {
        auto subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { i, row }, spriteSize);
        Ref<AnimationFrame> frame = CreateRef<AnimationFrame>(subTexture, frameDuration); // TODO : frame duration for each frame
        animation->AddFrame(frame);
    }
    m_Animator->AddAnimation(animation);
}
