#include "SandboxLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
    : m_CameraController(1280.0f / 720.f, true)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    m_TriVertexArray = VertexArray::Create();

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };

    Ref<VertexBuffer> triVertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
    BufferLayout triLayout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" }
    };
    triVertexBuffer->SetLayout(triLayout);
    m_TriVertexArray->AddVertexBuffer(triVertexBuffer);

    uint32_t triIndices[3] = { 0, 1, 2 };
    Ref<IndexBuffer> triIndexBuffer = IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t));
    m_TriVertexArray->SetIndexBuffer(triIndexBuffer);

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

    std::string vertexSrc = R"(
        #version 330 core
            
        layout(location = 0) in vec3 a_Position;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec3 v_Position;

        void main()
        {
            v_Position = a_Position;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
        }
    )";

    std::string fragmentSrc = R"(
        #version 330 core
            
        layout(location = 0) out vec4 color;

        in vec3 v_Position;

        void main()
        {
            color = vec4(v_Position * 0.5 + 0.5, 1.0);
        }
    )";

    m_VertexPosShader = Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

    m_ShaderLibrary.Load("FlatColor", "assets/shaders/FlatColor.glsl");
    auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

    m_Texture = Texture2D::Create("assets/textures/checkerboard.png");
    m_CharacterSprite = Texture2D::Create("assets/textures/emily-pokemon-style.png");

    std::dynamic_pointer_cast<OpenGLShader>(textureShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnEvent(Event& event)
{
    m_CameraController.OnEvent(event);
}

void SandboxLayer::OnUpdate(Timestep timestep)
{
    // Update
    m_CameraController.OnUpdate(timestep);

    // Render
    RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
    RenderCommand::Clear();

    Renderer::BeginScene(m_CameraController.GetCamera());

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
    auto textureShader = m_ShaderLibrary.Get("Texture");
    std::dynamic_pointer_cast<OpenGLShader>(flatColorShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_QuadColor);

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            Renderer::Submit(flatColorShader, m_QuadVertexArray, transform);
        }
    }

    m_Texture->Bind();
    Renderer::Submit(textureShader, m_QuadVertexArray);
    m_CharacterSprite->Bind();
    Renderer::Submit(textureShader, m_QuadVertexArray);

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
    Renderer::Submit(m_VertexPosShader, m_TriVertexArray, translation);

    Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_QuadColor));
    ImGui::End();
}