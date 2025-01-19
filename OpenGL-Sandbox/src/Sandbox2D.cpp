#include "Sandbox2D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

// TODO : do we need these includes?
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(CreateRef<TargetCamera2DController>(1280.f / 720.0f, -2.0f, 2.0f))
{
    MapParser::Init();
}

void Sandbox2D::OnAttach()
{
    PROFILE_FUNCTION();

    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    m_CameraController->SetZoomLevel(5.5f);

    m_GameMap = MapParser::GetInstance().Load("assets/2D/tilemaps/forest-town.tmx");

    float mapWidth = 39.4f; // TODO : magic number
    float mapHeight = 48.f; // TODO : magic number
    float mapBounds[4] = { -0.5f, mapWidth, -0.5f, mapHeight };
    m_CameraController->SetBounds(mapBounds);

    m_Player = CreateRef<PlayerController>(glm::vec3(18.f, 3.2f, 0.5f), "assets/2D/tilesets/trainer-sapphire.png");
    m_Player->LoadAssets();
}

void Sandbox2D::OnDetach()
{
    PROFILE_FUNCTION();

    MapParser::Shutdown();
}

void Sandbox2D::OnUpdate(GLCore::Timestep timestep)
{
    PROFILE_FUNCTION();

    // Update

    m_Player->OnUpdate(timestep);
    m_GameMap->OnUpdate(timestep);
    m_GameMap->SolveCollision(*m_Player);
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
        Renderer2D::BeginScene(m_CameraController->GetCamera());
        m_Player->OnRender();
        m_GameMap->OnRender();
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

    ImGui::End();
}

void Sandbox2D::OnEvent(GLCore::Event& event)
{
    m_CameraController->OnEvent(event);
}
