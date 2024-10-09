#include "Sandbox2D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = 
"PPWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWGGGGWWWWWWWWWWWWW"
"WWWWWWGGGGGGGWWWWWWWWWWW"
"WWWWWWGGGGGGGGGGWWWWWWWW"
"WWWWWWWWGGGGGGWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.f / 720.0f, false, false, -2.0f, 2.0f)
{
}

void Sandbox2D::OnAttach()
{
    PROFILE_FUNCTION();

    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    m_CameraController.SetZoomLevel(5.5f);

    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

    m_TrainerSpriteSheet = Texture2D::Create("assets/textures/trainer-sapphire.png");
    m_TrainerTexture = SubTexture2D::CreateFromCoords(m_TrainerSpriteSheet, { 0, 3 }, { 32, 48 });
    m_TilesetOutside = Texture2D::Create("assets/textures/outside.png");

    // grass
    m_TextureMap['G'] = SubTexture2D::CreateFromCoords(m_TilesetOutside, { 1, 501 }, { 32, 32 });
    // water
    m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_TilesetOutside, { 5, 300 }, {32, 32});
}

void Sandbox2D::OnDetach()
{
    PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(GLCore::Timestep timestep)
{
    PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(timestep);

    // Render
    {
        PROFILE_SCOPE("Renderer Prep");
        RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
        RenderCommand::Clear();
    }

    Renderer2D::ResetStats();
    {
        PROFILE_SCOPE("Renderer Draw");
        Renderer2D::BeginScene(m_CameraController.GetCamera());
        Renderer2D::DrawQuad({ -2.0f, -2.0f, 0.0f }, { 1.0f, 1.5f }, m_TrainerTexture);

        // inner loop is x so we read memory how it is laid out in memory
        for (uint32_t y = 0; y < m_MapHeight; y++)
        {
            for (uint32_t x = 0; x < m_MapWidth; x++)
            {
                char tileType = s_MapTiles[x + y * m_MapWidth];
                if (m_TextureMap.find(tileType) != m_TextureMap.end())
                    Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, -0.05f }, { 1.0f, 1.0f }, m_TextureMap[tileType]);
                else
                    Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, -0.05f }, { 1.0f, 1.0f }, m_TextureErrorColor);
            }
        }
        Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");

    auto stats = Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    // ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(GLCore::Event& event)
{
    m_CameraController.OnEvent(event);
}
