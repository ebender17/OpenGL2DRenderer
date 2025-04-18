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
    framebuffferSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
    framebuffferSpec.Width = INITIAL_SCREEN_WIDTH;
    framebuffferSpec.Height = INITIAL_SCREEN_HEIGHT;
    m_Framebuffer = GLCore::Framebuffer::Create(framebuffferSpec);
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

    m_GameMap = MapParser::GetInstance().Load("assets/tilemaps/forest-town.tmx");
    float mapWidth = 39.4f; // TODO : magic number
    float mapHeight = 48.f; // TODO : magic number
    float mapBounds[4] = { 0.0f, mapWidth, 0.0f, mapHeight };
    m_CameraController->SetBounds(mapBounds);

    m_Player = CreateRef<PlayerController>(glm::vec3(18.f, 3.2f, 0.5f), glm::vec2(1.0f, 1.5f),
        32, 48, "assets/tilesets/trainer-sapphire.png", m_GameMap);
    m_Player->LoadAssets();
    m_PlayerDebugBox = CreateRef<LineBox2D>();

    m_WaterReflectionShader = Shader::Create("assets/shaders/BatchedTexture.glsl", { "WATER_REFLECTION" });
    m_WaterReflectionShader->Bind();
    m_WaterReflectionShader->SetFloat3("u_WaterReflectionColor", { 0.0, 0.369, 1.0 });
    m_WaterReflectionShader->SetFloat("u_WaterReflectionColorMix", 0.31);
    m_WaterReflectionShader->SetFloat("u_WaterReflectionAlpha", 0.75);
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

    Renderer2D::ResetStats();
    RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
    RenderCommand::Clear();
    Renderer2D::BeginScene(m_CameraController->GetCamera());

    // 1. Render Water
    m_GameMap->OnRenderWaterLayer();
    // 2. Render Sprites, flipped upside-down above the water level and with water effects
    Renderer2D::SetShader(m_WaterReflectionShader);
    m_WaterReflectionShader->SetFloat("u_Time", glfwGetTime());
    m_Player->OnRenderReflection();
    Renderer2D::SetShader();
    // 3. Draw the ground
    m_GameMap->OnRender();
    // 4. Draw the player
    m_Player->OnRender();

    Renderer2D::EndScene();

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
