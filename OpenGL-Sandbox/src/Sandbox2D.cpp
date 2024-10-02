#include "Sandbox2D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWW"
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

    m_CheckerboardTexture = Texture2D::Create("assets/textures/checkerboard.png");
    m_PlayerTexture = Texture2D::Create("assets/textures/emily-pokemon-style.png");
    m_TilesetWater = Texture2D::Create("assets/textures/tiles-water.png");
    m_TilesetGrass = Texture2D::Create("assets/textures/tiles-grass.png");

    // grass
    m_TextureMap['G'] = SubTexture2D::CreateFromCoords(m_TilesetGrass, { 1, 1 }, { 16, 16 });
    // water
    m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_TilesetWater, {1, 1}, {16, 16});
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
        RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
        RenderCommand::Clear();
    }

    Renderer2D::ResetStats();
    {
        PROFILE_SCOPE("Renderer Draw");
        Renderer2D::BeginScene(m_CameraController.GetCamera());
        // Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), m_QuadColor);
        // Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        // Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
        Renderer2D::DrawQuad({ -2.0f, -2.0f, 0.0f }, { 1.0f, 1.0f }, m_PlayerTexture);

        /* for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.6f, (y + 5.0f) / 10.0f, 0.7f };
                Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        } */

        // inner loop is x so we read memory how it is laid out in memory
        for (uint32_t y = 0; y < m_MapHeight; y++)
        {
            for (uint32_t x = 0; x < m_MapWidth; x++)
            {
                char tileType = s_MapTiles[x + y + m_MapWidth];
                Ref<SubTexture2D> texture;
                if (m_TextureMap.find(tileType) != m_TextureMap.end())
                    texture = m_TextureMap[tileType];
                // TODO : else -> render error texture

                Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, -0.05f }, { 1.0f, 1.0f }, texture);
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
