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
    m_IdleSprite = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 3 }, { 32, 48 });
    
    // WALK FORWARD
    std::string name = "walk-forward";
    Ref<AnimationTopDown> walkForwardAnim = CreateRef<AnimationTopDown>(name, true);
    Ref<AnimationFrame> walkForwardFrame = CreateRef<AnimationFrame>(m_IdleSprite, 2.0f);
    walkForwardAnim->AddFrame(walkForwardFrame);

    Ref<SubTexture2D> walkSpriteTwo = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 3 }, { 32, 48 });
    Ref<AnimationFrame> walkForwardFrameTwo = CreateRef<AnimationFrame>(walkSpriteTwo, 2.0f);
    walkForwardAnim->AddFrame(walkForwardFrameTwo);

    Ref<SubTexture2D> walkSpriteThree = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 3 }, { 32, 48 });
    Ref<AnimationFrame> walkForwardFrameThree = CreateRef<AnimationFrame>(walkSpriteThree, 2.0f);
    walkForwardAnim->AddFrame(walkForwardFrameThree);

    Ref<SubTexture2D> walkSpriteFour = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 3 }, { 32, 48 });
    Ref<AnimationFrame> walkForwardFrameFour = CreateRef<AnimationFrame>(walkSpriteFour, 2.0f);
    walkForwardAnim->AddFrame(walkForwardFrameFour);

    m_Animator = CreateRef<AnimatorTopDown>();
    m_Animator->AddAnimation(walkForwardAnim);

    // TODO : replace where need to set animation
    m_Animator->SetActiveAnimation(walkForwardAnim->GetName());

    // TODO - left off here - add to Animator
    // 1. Make the AnimationFrames (subtexture, timeafteranim)
    // 2. Add the frames to a AnimationTopDown (name, loop) & add frames
    // 3. Add the anims to Animator
    // 4. Call animator.onupdate
    // 5. Set active animation where needed on input
}

void PlayerTopDown::OnUpdate(GLCore::Timestep timestep)
{
    m_Animator->OnUpdate(timestep);

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
    Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.5f }, m_IdleSprite);
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
