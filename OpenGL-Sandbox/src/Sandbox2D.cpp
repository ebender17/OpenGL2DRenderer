#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

using namespace GLCore;

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
    m_QuadVA = VertexArray::Create();

    float quadVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    Ref<VertexBuffer> quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
    quadVB->SetLayout({
        { ShaderDataType::Float3, "a_Position" }
    });
    m_QuadVA->AddVertexBuffer(quadVB);

    uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
    m_QuadVA->SetIndexBuffer(quadIB);

    m_FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
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

    Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_QuadColor);

    Renderer::Submit(m_FlatColorShader, m_QuadVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    Renderer::EndScene();
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
