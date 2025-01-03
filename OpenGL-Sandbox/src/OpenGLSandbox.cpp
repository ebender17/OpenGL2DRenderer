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

    InitLights();
    InitCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // vertex position, tex coords, normals
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    };

    // Setup buffers
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load and create textures
    GenerateTexture2D("assets/textures/wooden-box-diffuse.png", &m_Material.DiffuseId);
    GenerateTexture2D("assets/textures/wooden-box-specular.png", &m_Material.SpecularId);
    // GenerateTexture2D("assets/textures/box-emissive.png", &m_Material.EmissionId);

    // Shaders
    m_FlatColorShader = std::make_unique<OpenGLShader>("assets/shaders/FlatColor.glsl");
    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat4("u_Color", { 0.0f, 0.8f, 0.9f, 1.0f });

    m_Shader = std::make_unique<OpenGLShader>("assets/shaders/BasicDirLightOnly.glsl");
    m_Shader->Bind();
    m_Shader->SetInt("u_Material.diffuse", 0);
    m_Shader->SetInt("u_Material.specular", 1);
    // m_Shader->SetInt("u_Material.emission", 2);
    m_Shader->SetFloat("u_Material.shininess", 32.0f);

    // Shader lighting uniforms
    m_Shader->SetFloat3("u_DirLight.direction", m_DirectionalLight->Direction);
    m_Shader->SetFloat3("u_DirLight.ambient", m_DirectionalLight->Ambient);
    m_Shader->SetFloat3("u_DirLight.diffuse", m_DirectionalLight->Diffuse);
    m_Shader->SetFloat3("u_DirLight.specular", m_DirectionalLight->Specular);

    glGenVertexArrays(1, &m_LightCubeVAO);
    glBindVertexArray(m_LightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // TODO : need glBufferData?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void OpenGLSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
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
    m_Shader->SetFloat3("u_ViewPos", camPos);
    m_Shader->SetFloat("u_Time", glfwGetTime());

    // TODO : draw floor

    // First render pass. Draw objects as normal and write to stencil buffer.
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF); // enable writing to stencil buffer

    glBindVertexArray(m_VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Material.DiffuseId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Material.SpecularId);
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, m_Material.EmissionId);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_Shader->SetMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f); // TODO : use different matrix so we can re-use translate below?
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    m_Shader->SetMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    

    // Second render pass. Draw slightly scaled version of objects without writing to stencil buffer.
    // Only draw parts of object where stencil buffer does not equal 1. Gives us an outline.
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to stencil buffer
    glDisable(GL_DEPTH_TEST);
    m_FlatColorShader->Bind();
    m_FlatColorShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    float scale = 1.1f;

    model = glm::mat4(1.0f);
    model = glm::scale(model, { scale, scale, scale }) * 
        glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_FlatColorShader->SetMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::scale(model, { scale, scale, scale }) * 
        glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    m_FlatColorShader->SetMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLSandbox::OnImGuiRender()
{
}

void OpenGLSandbox::OnEvent(GLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(OpenGLSandbox::OnWindowResized));
}

void OpenGLSandbox::InitLights()
{
    m_DirectionalLight = std::make_unique<DirectionalLight>(glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.3f, 0.24f, 0.14f),
        glm::vec3(0.7f, 0.42f, 0.26f),
        glm::vec3(0.5f, 0.5f, 0.5f));

}

void OpenGLSandbox::InitCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

void OpenGLSandbox::GenerateTexture2D(const std::string& filepath, uint32_t* texture)
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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