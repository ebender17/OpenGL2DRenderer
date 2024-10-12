#include "PlayerTopDown.h"

using namespace GLCore;

PlayerTopDown::PlayerTopDown(const glm::vec3& position, const char* textureFilepath)
    : m_Position(position), m_TextureFilepath(textureFilepath)
{
}

PlayerTopDown::~PlayerTopDown()
{
}

void PlayerTopDown::LoadAssets()
{
    m_SpriteSheet = Texture2D::Create(m_TextureFilepath);

    // TODO - left off here - add to Animator
    // 1. Make the AnimationFrames (subtexture, timeafteranim)
    // 2. Add the frames to a AnimationTopDown (name, loop) & add frames
    // 3. Add the anims to Animator
    // 4. Call animator.onupdate
    // 5. Set active animation where needed on input
    m_Sprite = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 3 }, { 32, 48 }); // TODO : change from hard-coded
}

void PlayerTopDown::OnUpdate(GLCore::Timestep timestep)
{
    if (!m_IsMoving)
        ProcessPlayerInput();
    else if (m_InputDirection != glm::vec2(0.0f))
        Move(timestep);
    else
        m_IsMoving = false;
}

void PlayerTopDown::OnRender()
{
    // TODO : animate sprite & update based on facing position & state
    Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.5f }, m_Sprite);
}

void PlayerTopDown::ProcessPlayerInput()
{
    m_InputDirection = glm::vec2(0.0f);

    if (m_InputDirection.y == 0 && Input::IsKeyPressed(Key::A))
    {
        m_InputDirection = { -1.0f, 0.0f };
    }
    if (m_InputDirection.y == 0 && Input::IsKeyPressed(Key::D))
    {
        m_InputDirection = { 1.0f, 0.0f };
    }
    if (m_InputDirection.x == 0 && Input::IsKeyPressed(Key::W))
    {
        m_InputDirection = { 0.0f, 1.0f };
    }
    if (Input::IsKeyPressed(Key::S))
    {
        m_InputDirection = { 0.0f, -1.0f };
    }

    if (m_InputDirection != glm::vec2(0.0f))
    {
        m_InitialPosition = m_Position;
        m_IsMoving = true;
    }
}

void PlayerTopDown::Move(Timestep timestep)
{
    m_PercentMovedToNextTile += m_Speed * timestep;
    if (m_PercentMovedToNextTile >= 1.0)
    {
        m_Position.x = m_InitialPosition.x + (TILE_SIZE * m_InputDirection.x);
        m_Position.y = m_InitialPosition.y + (TILE_SIZE * m_InputDirection.y);
        m_PercentMovedToNextTile = 0.0f;
        m_IsMoving = false;
    }
}
