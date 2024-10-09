#include "RawOpenGLSandbox.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"
#include "Platform/OpenGL/Renderer/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLCore;

RawOpenGLSandbox::RawOpenGLSandbox()
    : Layer("Sandbox2D"), m_CameraController(1280.f / 720.0f, false, false, -2.0f, 2.0f)
{
}

void RawOpenGLSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left
    };

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    m_Shader = std::make_unique<OpenGLShader>("assets/shaders/Basic.glsl");
    m_Shader->Bind();
    m_Shader->SetFloat4("u_Color", glm::vec4(0.0f, 0.5f, 0.8f, 1.0f));

    glBindVertexArray(0);
}

void RawOpenGLSandbox::OnDetach()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void RawOpenGLSandbox::OnUpdate(GLCore::Timestep timestep)
{
    m_CameraController.OnUpdate(timestep);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->Bind();
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RawOpenGLSandbox::OnImGuiRender()
{
}

void RawOpenGLSandbox::OnEvent(GLCore::Event& event)
{
}
