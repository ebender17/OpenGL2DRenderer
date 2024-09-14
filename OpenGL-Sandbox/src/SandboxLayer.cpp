#include "SandboxLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
    :m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    m_QuadVertexArray = VertexArray::Create();
    
    float quadVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    };

    Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
    quadVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" }
    });
    m_QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

    uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
    m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

    m_FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");

    m_TextureShader = Shader::Create("assets/shaders/Texture.glsl");

    m_Texture = Texture2D::Create("assets/textures/checkerboard.png");
    m_CharacterSprite = Texture2D::Create("assets/textures/emily-pokemon-style.png");

    std::dynamic_pointer_cast<OpenGLShader>(m_TextureShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnEvent(Event& event)
{
    if (event.GetEventType() == EventType::KeyPressed)
    {
        KeyPressedEvent& e = (KeyPressedEvent&)event;
        // keycodes line up with ASCII so can cast to char to print char pressed
        LOG_TRACE("{0}", (char)e.GetKeyCode());
    }
}

void SandboxLayer::OnUpdate(Timestep timestep)
{
    if (Input::IsKeyPressed(Key::Tab))
        LOG_TRACE("Tab key is pressed!");

    RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
    RenderCommand::Clear();

    m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
    // m_Camera.SetRotation(45.0f);

    Renderer::BeginScene(m_Camera);

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_QuadColor);

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            Renderer::Submit(m_FlatColorShader, m_QuadVertexArray, transform);
        }
    }

    m_Texture->Bind();
    Renderer::Submit(m_TextureShader, m_QuadVertexArray);
    m_CharacterSprite->Bind();
    Renderer::Submit(m_TextureShader, m_QuadVertexArray);

    Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_QuadColor));
    ImGui::End();
}