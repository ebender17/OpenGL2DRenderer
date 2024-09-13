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

    m_VertexArray = VertexArray::Create();

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.239f, 0.352f, 0.945f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.133f, 0.819f, 0.933f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.886f, 0.952f, 0.960f, 1.0f
    };

    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color"}
    };
    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    uint32_t indices[3] = { 0, 1, 2 };
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(indexBuffer);

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

    std::string vertexSource = R"(
        #version 330 core
        
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;
        
        out vec4 v_Color;

        void main()
        {
            v_Color = a_Color;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

    std::string fragmentSource = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec4 v_Color;

        void main()
        {
            color = v_Color;
        }
    )";

    m_Shader = Shader::Create(vertexSource, fragmentSource);

    std::string flatColorVertexSource = R"(
        #version 330 core
        
        layout(location = 0) in vec3 a_Position;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        void main()
        {
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

    std::string flatColorFragmentSource = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        uniform vec3 u_Color;

        void main()
        {
            color = vec4(u_Color, 1.0);
        }
    )";

    m_FlatColorShader = Shader::Create(flatColorVertexSource, flatColorFragmentSource);

    std::string textureVertexSource = R"(
        #version 330 core
        
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec2 a_TexCoord;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec2 v_TexCoord;

        void main()
        {
            v_TexCoord = a_TexCoord;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

    std::string textureFragmentSource = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec2 v_TexCoord;

        uniform sampler2D u_Texture;

        void main()
        {
            color = texture(u_Texture, v_TexCoord);
        }
    )";

    m_TextureShader = Shader::Create(textureVertexSource, textureFragmentSource);

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
    if (Input::IsKeyPressed(GLCORE_KEY_TAB))
        LOG_TRACE("Tab key is pressed!");

    // auto [x, y] = Input::GetMousePosition();
    // LOG_INFO("{0}, {1}", x, y);

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
    // Renderer::Submit(m_Shader, m_VertexArray);

    Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_QuadColor));
    ImGui::End();
}