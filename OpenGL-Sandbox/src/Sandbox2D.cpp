#include "Sandbox2D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = 
"PPGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGWWWWGGGGGGGGGGG"
"GGGGGGGGWWWWWWWGGGGGGGGG"
"GGGGGGGWWWWWWWWWGGGGGGGG"
"GGGGGGGWWWWWWWWWGGGGGGGG"
"GGGGGGGGWWWWWWWGGGGGGGGG"
"GGGGGGGGGGWWWWWGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
;

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(CreateRef<TargetCamera2DController>(1280.f / 720.0f, -2.0f, 2.0f))
{
}

void Sandbox2D::OnAttach()
{
    PROFILE_FUNCTION();

    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    m_CameraController->SetZoomLevel(5.5f);

    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

    // TODO : figure out the actual bounds we want, left off here
    float mapBounds[4] = { 0.0f, m_MapWidth, 0.0f, m_MapHeight };
    m_CameraController->SetBounds(mapBounds);

    m_Player = CreateRef<PlayerController>(glm::vec3(0.0f, 0.0f, 0.5f), "assets/textures/trainer-sapphire.png");
    m_Player->LoadAssets();
    m_TilesetOutside = Texture2D::Create("assets/textures/outside.png");

    // grass
    m_TexCoordsMap['G'] = SubTexture2D::CreateFromCoords(m_TilesetOutside, { 1, 501 }, { 32, 32 });
    // water
    m_TexCoordsMap['W'] = SubTexture2D::CreateFromCoords(m_TilesetOutside, { 6, 414 }, {32, 32});
}

void Sandbox2D::OnDetach()
{
    PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(GLCore::Timestep timestep)
{
    PROFILE_FUNCTION();

    // Update

    m_Player->OnUpdate(timestep);
    m_CameraController->SetTarget(m_Player->GetPosition());
    m_CameraController->OnUpdate(timestep);

    // Render
    {
        PROFILE_SCOPE("Renderer Prep");
        RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
        RenderCommand::Clear();
    }

    Renderer2D::ResetStats();
    {
        PROFILE_SCOPE("Renderer Draw");
        // TODO : New Camera Controller
        Renderer2D::BeginScene(m_CameraController->GetCamera());
        m_Player->OnRender();

        // inner loop is x so we read memory how it is laid out in memory
        for (uint32_t y = 0; y < m_MapHeight; y++)
        {
            for (uint32_t x = 0; x < m_MapWidth; x++)
            {
                char tileType = s_MapTiles[x + y * m_MapWidth];
                if (m_TexCoordsMap.find(tileType) != m_TexCoordsMap.end())
                    // TODO : figure out where we want to start setting tiles & go back to flipping w/code below 
                    // x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f
                    Renderer2D::DrawQuad({ x, y, -0.05f }, { 1.0f, 1.0f }, m_TexCoordsMap[tileType]->GetTexture(), m_TexCoordsMap[tileType]->GetTexCoords());
                else
                    Renderer2D::DrawQuad({ x, y, -0.05f }, { 1.0f, 1.0f }, m_TextureErrorColor);
            }
        }

        // inner loop is x so we read memory how it is laid out in memory
        for (uint32_t y = 0; y < m_MapHeight; y++)
        {
            for (uint32_t x = 0; x < m_MapWidth; x++)
            {
                char tileType = s_MapTiles[x + y * m_MapWidth];
                if (m_TexCoordsMap.find(tileType) != m_TexCoordsMap.end())
                    // TODO : figure out where we want to start setting tiles & go back to flipping w/code below 
                    // x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f
                    Renderer2D::DrawQuad({ x, y, -0.05f }, { 1.0f, 1.0f }, m_TexCoordsMap[tileType]->GetTexture(), m_TexCoordsMap[tileType]->GetTexCoords());
                else
                    Renderer2D::DrawQuad({ x, y, -0.05f }, { 1.0f, 1.0f }, m_TextureErrorColor);
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
    m_CameraController->OnEvent(event);
}
