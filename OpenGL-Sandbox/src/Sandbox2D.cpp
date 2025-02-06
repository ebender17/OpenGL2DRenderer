#include "Sandbox2D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

// TODO : do we need these includes?
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;

const float INITIAL_SCREEN_WIDTH = 1280.f;
const float INITIAL_SCREEN_HEIGHT = 720.f;

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(CreateRef<TargetCamera2DController>(INITIAL_SCREEN_WIDTH / INITIAL_SCREEN_HEIGHT, -2.0f, 2.0f))
{
    MapParser::Init();
}

void Sandbox2D::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    m_CameraController->SetZoomLevel(5.5f);

    m_GameMap = MapParser::GetInstance().Load("assets/tilemaps/forest-town.tmx");
    float mapWidth = 39.4f; // TODO : magic number
    float mapHeight = 48.f; // TODO : magic number
    float mapBounds[4] = { -0.5f, mapWidth, -0.5f, mapHeight };
    m_CameraController->SetBounds(mapBounds);

    m_Player = CreateRef<PlayerController>(glm::vec3(18.f, 3.2f, 0.5f), glm::vec2(1.0f, 1.5f),
        32, 48, "assets/tilesets/trainer-sapphire.png", m_GameMap);
    m_Player->LoadAssets();
    m_PlayerDebugBox = CreateRef<LineBox2D>();
}

void Sandbox2D::OnDetach()
{
    MapParser::Shutdown();
}

void Sandbox2D::OnUpdate(GLCore::Timestep timestep)
{
    // Update
    m_GameMap->OnUpdate(timestep);
    m_Player->OnUpdate(timestep);
    m_CameraController->SetTarget(m_Player->GetPosition());
    m_CameraController->OnUpdate(timestep);

    // Render
    RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
    RenderCommand::Clear();

    Renderer2D::ResetStats();
    Renderer2D::BeginScene(m_CameraController->GetCamera());
    m_Player->OnRender();
    m_GameMap->OnRender();
    Renderer2D::EndScene();
    m_PlayerDebugBox->DrawCollider(*m_Player, *m_CameraController);
    // TODO : add line rendering into Renderer2D instead of DebugBox2D?
    // something like Renderer2D::DrawBox
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
