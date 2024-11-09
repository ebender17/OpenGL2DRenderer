#include "RawOpenGLSandbox.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"
#include "Platform/OpenGL/Renderer/OpenGLShader.h"

#include <imgui/imgui.h>

#include <stb_image/stb_image.h>

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

    // Setup buffers
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

    // Load and create textures
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    stbi_uc* data = nullptr;
    {
        PROFILE_SCOPE("stbi load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");

        data = stbi_load("assets/textures/checkerboard.png", &width, &height, &channels, 0);
    }
    GLCORE_ASSERT(data, "Failed to load image!");

    GLenum internalFormat, dataFormat;
    switch (channels)
    {
    case 1:
        GLCORE_ASSERT(data, "Do not yet support textures with 1 channel.");
        break;
    case 2:
        GLCORE_ASSERT(data, "Do not yet support textures with 2 channels.");
        break;
    case 3:
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
        break;
    case 4:
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
        break;
    default:
        GLCORE_ASSERT(data, "Do not support textures with more than 4 channels.");
    }

    glGenTextures(1, &m_CheckerboardTexture);
    glBindTexture(GL_TEXTURE_2D, m_CheckerboardTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // Shaders
    m_Shader = std::make_unique<OpenGLShader>("assets/shaders/Basic.glsl");
    m_Shader->Bind();
    m_Shader->SetFloat4("u_Color", glm::vec4(0.0f, 0.5f, 0.8f, 1.0f));

    glBindVertexArray(0);
}

void RawOpenGLSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void RawOpenGLSandbox::OnUpdate(GLCore::Timestep timestep)
{
    m_CameraController.OnUpdate(timestep);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_CheckerboardTexture);

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
