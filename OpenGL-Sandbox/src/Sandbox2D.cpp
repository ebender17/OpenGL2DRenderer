#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.f / 720.0f, false, -2.0f, 2.0f)
{
}

void Sandbox2D::OnAttach()
{
    m_CheckerboardTexture = Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(GLCore::Timestep timestep)
{
    // Update
    m_CameraController.OnUpdate(timestep);

    // Render
    RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
    RenderCommand::Clear();

    Renderer2D::BeginScene(m_CameraController.GetCamera());
    Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.75f, 0.6f }, m_QuadColor);
    Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 5.0f, { 0.0f, 0.5f, 0.5f, 1.0f });
    Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(GLCore::Event& event)
{
    m_CameraController.OnEvent(event);
}
