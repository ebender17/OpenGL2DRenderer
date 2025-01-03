#include "OpenGLSandbox.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"
#include "Platform/OpenGL/Renderer/OpenGLShader.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <stb_image/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>

using namespace GLCore;

OpenGLSandbox::OpenGLSandbox()
    : Layer("OpenGLSandbox")
{
}

void OpenGLSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // vertex position, tex coords
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // positions, tex coords
    float quadVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    };

    m_VegetationPositions.reserve(5);
    m_VegetationPositions.emplace_back(glm::vec3(-1.5f, -0.1f, 0.5f));
    m_VegetationPositions.emplace_back(glm::vec3(1.5f, -0.1f, 1.2f));
    m_VegetationPositions.emplace_back(glm::vec3(0.0f, -0.1f, 0.7f));
    m_VegetationPositions.emplace_back(glm::vec3(-0.3f, -0.1f, -2.3f));
    m_VegetationPositions.emplace_back(glm::vec3(0.5f, -0.1f, -0.6f));

    // Setup buffers
    glGenVertexArrays(1, &m_CubeVAO);
    glBindVertexArray(m_CubeVAO);
    glGenBuffers(1, &m_CubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    glGenVertexArrays(1, &m_QuadVAO);
    glBindVertexArray(m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // Load and create textures
    GenerateTexture2D("assets/textures/tile.png", &m_TileTexture, GL_REPEAT);
    GenerateTexture2D("assets/textures/metal.png", &m_MetalTexture, GL_REPEAT);
    GenerateTexture2D("assets/textures/flat-grass-sprite.png", &m_GrassSpriteTexture, GL_CLAMP_TO_EDGE);

    // Shaders
    m_FlatColorShader = std::make_unique<OpenGLShader>("assets/shaders/FlatColor.glsl");
    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat4("u_Color", { 0.65f, 1.0f, 0.51f, 1.0f });
    m_Shader = std::make_unique<OpenGLShader>("assets/shaders/FlatTexture.glsl");
    m_Shader->Bind();
    m_Shader->SetInt("u_Texture", 0);
    m_AlphaClippedShader = std::make_unique<OpenGLShader>("assets/shaders/AlphaClippedTexture.glsl");
    m_AlphaClippedShader->Bind();
    m_AlphaClippedShader->SetFloat("u_AlphaThreshold", 0.1f);
    m_AlphaClippedShader->SetFloat4("u_Color", { 0.09f, 0.73f, 0.47f, 1.0f });

    glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void OpenGLSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteBuffers(1, &m_CubeVBO);
}

void OpenGLSandbox::OnUpdate(GLCore::Timestep timestep)
{
    // Update
    m_Camera->OnUpdate(timestep);

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glm::vec3 camPos = m_Camera->GetPosition();
    m_Shader->Bind();
    m_Shader->SetMat4("u_ViewProjection", viewProjectionMatrix);

    // Floor
    glStencilMask(0x00);
    glBindVertexArray(m_QuadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_MetalTexture);
    glm::mat4 modelFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 0.0f));
    modelFloor = glm::rotate(modelFloor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelFloor = glm::scale(modelFloor, glm::vec3(7.0f, 7.0f, 1.0f));
    m_Shader->SetMat4("u_Model", modelFloor);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw boxes
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF); // enable writing to stencil buffer
    glBindVertexArray(m_CubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TileTexture);
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_Shader->SetMat4("u_Model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f));
    m_Shader->SetMat4("u_Model", model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Draw outlines
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to stencil buffer
    glDisable(GL_DEPTH_TEST);
    m_FlatColorShader->Bind();
    m_FlatColorShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    float scale = 1.1f;
    model1 = glm::scale(model1, glm::vec3(scale, scale, scale));
    m_FlatColorShader->SetMat4("u_Model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model2 = glm::scale(model2, glm::vec3(scale, scale, scale));
    m_FlatColorShader->SetMat4("u_Model", model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    // Vegetation
    glBindVertexArray(m_QuadVAO);
    m_AlphaClippedShader->Bind();
    m_AlphaClippedShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GrassSpriteTexture);
    for (glm::vec3 position : m_VegetationPositions)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        m_AlphaClippedShader->SetMat4("u_Model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
}

void OpenGLSandbox::OnImGuiRender()
{
}

void OpenGLSandbox::OnEvent(GLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(OpenGLSandbox::OnWindowResized));
}

void OpenGLSandbox::InitCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

void OpenGLSandbox::GenerateTexture2D(const std::string& filepath, uint32_t* texture, uint32_t wrapOption)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    stbi_uc* data = nullptr;
    {
        PROFILE_SCOPE("stbi load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");

        data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
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

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

bool OpenGLSandbox::OnWindowResized(GLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}