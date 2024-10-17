#include "PlayerTopDown.h"

using namespace GLCore;

PlayerTopDown::PlayerTopDown(const glm::vec3& position, const char* textureFilepath)
    : m_Position(position), m_TextureFilepath(textureFilepath), m_InitialPosition(position)
{
}

PlayerTopDown::~PlayerTopDown()
{
}

void PlayerTopDown::LoadAssets()
{
    m_SpriteSheet = Texture2D::Create(m_TextureFilepath);

    // TODO : Clean up setting up animations, create function
    // IDLE DOWN
    idleDownAnim = CreateRef<AnimationTopDown>("idle-down", true);
    Ref<SubTexture2D> subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 3 }, { 32, 48 });
    Ref<AnimationFrame> frame = CreateRef<AnimationFrame>(subTexture, 0.0f);
    idleDownAnim->AddFrame(frame);

    // WALK DOWN
    walkDownAnim = CreateRef<AnimationTopDown>("walk-down", true);
    walkDownAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 3 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkDownAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 3 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkDownAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 3 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkDownAnim->AddFrame(frame);

    // IDLE UP
    idleUpAnim = CreateRef<AnimationTopDown>("idle-up", true);
    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 0 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.0f);
    idleUpAnim->AddFrame(frame);

    // WALK UP
    walkUpAnim = CreateRef<AnimationTopDown>("walk-up", true);
    walkDownAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 0 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkUpAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 0 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkUpAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 0 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkUpAnim->AddFrame(frame);

    // IDLE LEFT
    idleLeftAnim = CreateRef<AnimationTopDown>("idle-left", true);
    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 2 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.0f);
    idleLeftAnim->AddFrame(frame);

    // WALK LEFT
    walkLeftAnim = CreateRef<AnimationTopDown>("walk-left", true);
    walkLeftAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 2 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkLeftAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 2 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkLeftAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 2 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkLeftAnim->AddFrame(frame);

    // IDLE RIGHT
    idleRightAnim = CreateRef<AnimationTopDown>("idle-right", true);
    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 1 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.0f);
    idleRightAnim->AddFrame(frame);

    // WALK RIGHT
    walkRightAnim = CreateRef<AnimationTopDown>("walk-right", true);
    walkRightAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 1 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkRightAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkRightAnim->AddFrame(frame);

    subTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 1 }, { 32, 48 });
    frame = CreateRef<AnimationFrame>(subTexture, 0.15f);
    walkRightAnim->AddFrame(frame);

    m_Animator = CreateRef<AnimatorTopDown>();
    m_Animator->AddAnimation(idleDownAnim);
    m_Animator->AddAnimation(idleUpAnim);
    m_Animator->AddAnimation(idleLeftAnim);
    m_Animator->AddAnimation(idleRightAnim);
    m_Animator->AddAnimation(walkDownAnim);
    m_Animator->AddAnimation(walkUpAnim);
    m_Animator->AddAnimation(walkLeftAnim);
    m_Animator->AddAnimation(walkRightAnim);

    // TODO : replace where need to set animation
    m_Direction = Direction::Down;
    m_Animator->SetActiveAnimation(idleDownAnim->GetName());
}

void PlayerTopDown::OnUpdate(GLCore::Timestep timestep)
{
    m_Animator->OnUpdate(timestep);

    if (!m_IsMoving)
    {
        ProcessPlayerInput();
    }
    else if (m_InputDirection != glm::vec2(0.0f))
    {
        switch (m_Direction)
        {
            case Direction::Down:
                m_Animator->SetActiveAnimation(walkDownAnim->GetName());
                break;
            case Direction::Up:
                m_Animator->SetActiveAnimation(walkUpAnim->GetName());
                break;
            case Direction::Left:
                m_Animator->SetActiveAnimation(walkLeftAnim->GetName());
                break;
            case Direction::Right:
                m_Animator->SetActiveAnimation(walkRightAnim->GetName());
                break;
        }
        Move(timestep);
    }
}

void PlayerTopDown::OnRender()
{
    Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.5f }, m_Animator->GetCurrentFrame());
}

void PlayerTopDown::ProcessPlayerInput()
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
        m_IsMoving = true;
    }
    else
    {
        switch (m_Direction)
        {
        case Direction::Down:
            m_Animator->SetActiveAnimation(idleDownAnim->GetName());
            break;
        case Direction::Up:
            m_Animator->SetActiveAnimation(idleUpAnim->GetName());
            break;
        case Direction::Left:
            m_Animator->SetActiveAnimation(idleLeftAnim->GetName());
            break;
        case Direction::Right:
            m_Animator->SetActiveAnimation(idleRightAnim->GetName());
            break;
        }
    }
}

void PlayerTopDown::Move(Timestep timestep)
{
    m_PercentMovedToNextTile += m_Speed * timestep;
    if (m_PercentMovedToNextTile >= 1.0)
    {
        m_Position.x = m_InitialPosition.x + m_InputDirection.x;
        m_Position.y = m_InitialPosition.y + m_InputDirection.y;
        m_PercentMovedToNextTile = 0.0f;
        m_IsMoving = false;
    }
    else
    {
        m_Position.x = m_InitialPosition.x + m_InputDirection.x * m_PercentMovedToNextTile;
        m_Position.y = m_InitialPosition.y + m_InputDirection.y * m_PercentMovedToNextTile;
    }
}
