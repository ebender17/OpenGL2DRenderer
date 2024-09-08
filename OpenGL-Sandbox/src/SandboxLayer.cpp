#include "SandboxLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    m_VertexArray.reset(VertexArray::Create());

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.239f, 0.352f, 0.945f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.133f, 0.819f, 0.933f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.886f, 0.952f, 0.960f, 1.0f
    };

    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color"}
    };
    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    uint32_t indices[3] = { 0, 1, 2 };
    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_QuadVertexArray.reset(VertexArray::Create());
    
    float quadVertices[3 * 4] = {
        -0.75f, -0.75f, 0.0f,
         0.75f, -0.75f, 0.0f,
         0.75f,  0.75f, 0.0f,
        -0.75f,  0.75f, 0.0f
    };

    std::shared_ptr<VertexBuffer> quadVertexBuffer;
    quadVertexBuffer.reset(VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
    quadVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" }
    });
    m_QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

    uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
    std::shared_ptr<IndexBuffer> quadIndexBuffer;
    quadIndexBuffer.reset(IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
    m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

    std::string vertexSource = R"(
        #version 330 core
        
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        
        out vec4 v_Color;

        void main()
        {
            v_Color = a_Color;
            gl_Position = vec4(a_Position, 1.0);
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

    m_Shader.reset(new Shader(vertexSource, fragmentSource));

    std::string blueShaderVertexSource = R"(
        #version 330 core
        
        layout(location = 0) in vec3 a_Position;

        void main()
        {
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string blueShaderFragmentSource = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(0.0549, 0.0824, 0.227, 1.0);
        }
    )";

    m_BlueShader.reset(new Shader(blueShaderVertexSource, blueShaderFragmentSource));
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

    Renderer::BeginScene();

    Renderer::Submit(m_BlueShader, m_QuadVertexArray);
    Renderer::Submit(m_Shader, m_VertexArray);

    Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender()
{
}