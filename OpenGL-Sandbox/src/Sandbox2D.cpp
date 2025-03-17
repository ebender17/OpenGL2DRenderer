#include "Sandbox2D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <GLFW/glfw3.h>

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

    // TODO : move post process framebuffer & shader into Renderer2D?
    GLCore::FramebufferSpecification framebuffferSpec;
    framebuffferSpec.Width = INITIAL_SCREEN_WIDTH;
    framebuffferSpec.Height = INITIAL_SCREEN_HEIGHT;
    m_Framebuffer = GLCore::Framebuffer::Create(framebuffferSpec);
    m_FullscreenShader = Shader::Create("assets/shaders/WaterReflections.glsl");
    m_FullscreenShader->Bind();
    m_FullscreenShader->SetInt("u_SceneTexture", 0);
    m_FullscreenShader->SetInt("u_TileMap", 1);
    // TODO : set uniforms you can upfront

    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int quadVBO;
    glGenVertexArrays(1, &m_FullscreenVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(m_FullscreenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    glDeleteBuffers(1, &quadVBO);

    m_GameMap = MapParser::GetInstance().Load("assets/tilemaps/tilemap-test.tmx");
    float mapWidth = 39.4f; // TODO : magic number
    float mapHeight = 48.f; // TODO : magic number
    float mapBounds[4] = { 0.0f, mapWidth, 0.0f, mapHeight };
    m_CameraController->SetBounds(mapBounds);

    m_Player = CreateRef<PlayerController>(glm::vec3(18.f, 3.2f, 0.5f), glm::vec2(1.0f, 1.5f),
        32, 48, "assets/tilesets/trainer-sapphire.png", m_GameMap);
    m_Player->LoadAssets();
    m_PlayerDebugBox = CreateRef<LineBox2D>();
}

void Sandbox2D::OnDetach()
{
    MapParser::Shutdown();
    glDeleteVertexArrays(1, &m_FullscreenVAO);
}

void Sandbox2D::OnUpdate(GLCore::Timestep timestep)
{
    // Update
    m_GameMap->OnUpdate(timestep);
    m_Player->OnUpdate(timestep);
    m_CameraController->SetTarget(m_Player->GetPosition());
    m_CameraController->OnUpdate(timestep);

    // Render
    // First pass
    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
    RenderCommand::Clear();
    Renderer2D::BeginScene(m_CameraController->GetCamera());
    m_Player->OnRender();
    m_GameMap->OnRender();
    Renderer2D::EndScene();
    m_Framebuffer->Unbind();

    // Second pass
    // glDisable(GL_DEPTH_TEST); // TODO : figure out what is going on with depth test
    RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
    RenderCommand::Clear();
    // TODO : move post process framebuffer & shader into Renderer2D?
    m_FullscreenShader->Bind();
    glBindVertexArray(m_FullscreenVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorAttachment());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GameMap->GetTilemapTexture());

    // Pass the inverse projection-view matrix
    glm::mat4 invProjectionView = glm::inverse(m_CameraController->GetCamera().GetViewProjectionMatrix());
    m_FullscreenShader->SetMat4("u_InvProjectionView", invProjectionView);
    // Set tile size in world space
    m_FullscreenShader->SetFloat("u_TileSize", 1.0f); // TODO : make sure this is correct

    m_FullscreenShader->SetFloat("u_WaveAmplitude", 0.01f);
    m_FullscreenShader->SetFloat("u_WaveFrequency", 10.0f);
    m_FullscreenShader->SetFloat("u_ReflectionAlpha", 0.6f);
    m_FullscreenShader->SetFloat4("u_WaterTint", { 1.0f, 1.0f, 1.0f, 0.3f });
    m_FullscreenShader->SetFloat("u_Time", glfwGetTime());

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // m_PlayerDebugBox->DrawCollider(*m_Player, *m_CameraController);
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
