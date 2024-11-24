#include "RawOpenGLSandbox.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"
#include "Platform/OpenGL/Renderer/OpenGLShader.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <stb_image/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>

using namespace GLCore;

RawOpenGLSandbox::RawOpenGLSandbox()
    : Layer("Sandbox2D")
{
}

void RawOpenGLSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitLights();
    InitCamera();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    GenerateTexture2D("assets/textures/box-emissive.png", &m_Material.EmissionId);

    // Shaders
    m_FlatColorShader = std::make_unique<OpenGLShader>("assets/shaders/FlatColor.glsl");
    m_FlatColorShader->Bind();

    m_Shader = std::make_unique<OpenGLShader>("assets/shaders/Basic.glsl");
    m_Shader->Bind();
    m_Shader->SetInt("u_Material.diffuse", 0);
    m_Shader->SetInt("u_Material.specular", 1);
    m_Shader->SetInt("u_Material.emission", 2);
    m_Shader->SetFloat("u_Material.shininess", 32.0f);

    // Shader lighting uniforms
    m_Shader->SetFloat3("u_DirLight.direction", m_DirectionalLight->Direction);
    m_Shader->SetFloat3("u_DirLight.ambient", m_DirectionalLight->Ambient);
    m_Shader->SetFloat3("u_DirLight.diffuse", m_DirectionalLight->Diffuse);
    m_Shader->SetFloat3("u_DirLight.specular", m_DirectionalLight->Specular);

    for (int i = 0; i < 4; i++)
    {
        m_Shader->SetFloat3(std::format("u_PointLights[{}].position", i), m_PointLights[i]->Position);
        m_Shader->SetFloat3(std::format("u_PointLights[{}].ambient", i), m_PointLights[i]->Ambient);
        m_Shader->SetFloat3(std::format("u_PointLights[{}].diffuse", i), m_PointLights[i]->Diffuse);
        m_Shader->SetFloat3(std::format("u_PointLights[{}].specular", i), m_PointLights[i]->Specular);
        m_Shader->SetFloat(std::format("u_PointLights[{}].constant", i), m_PointLights[i]->Constant);
        m_Shader->SetFloat(std::format("u_PointLights[{}].linear", i),m_PointLights[i]->Linear);
        m_Shader->SetFloat(std::format("u_PointLights[{}].quadratic", i), m_PointLights[i]->Quadratic);
    }

    m_Shader->SetFloat3("u_SpotLight.ambient", m_FlashLight->Ambient);
    m_Shader->SetFloat3("u_SpotLight.diffuse", m_FlashLight->Diffuse);
    m_Shader->SetFloat3("u_SpotLight.specular", m_FlashLight->Specular);
    m_Shader->SetFloat("u_SpotLight.constant", m_FlashLight->Constant);
    m_Shader->SetFloat("u_SpotLight.linear", m_FlashLight->Linear);
    m_Shader->SetFloat("u_SpotLight.quadratic", m_FlashLight->Quadratic);
    m_Shader->SetFloat("u_SpotLight.innerCutOff", m_FlashLight->InnerCutOff);
    m_Shader->SetFloat("u_SpotLight.outerCutOff", m_FlashLight->OuterCutOff);

    glGenVertexArrays(1, &m_LightCubeVAO);
    glBindVertexArray(m_LightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
    // Update
    m_Camera->OnUpdate(timestep);

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glm::vec3 camPos = m_Camera->GetPosition();
    m_Shader->Bind();
    m_Shader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    m_Shader->SetFloat3("u_ViewPos", camPos);
    m_Shader->SetFloat("u_Time", glfwGetTime());

    m_Shader->SetFloat3("u_SpotLight.position", camPos);
    m_Shader->SetFloat3("u_SpotLight.direction", m_Camera->GetForward());

    // Model matrix
    // TODO : move outside of update loop
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Material.DiffuseId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Material.SpecularId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Material.EmissionId);

    glBindVertexArray(m_VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_Shader->SetMat4("u_Model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    m_FlatColorShader->Bind();
    m_FlatColorShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    glBindVertexArray(m_LightCubeVAO);
    for (unsigned int i = 0; i < m_PointLights.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_PointLights[i]->Position);
        model = glm::scale(model, glm::vec3(0.2f));
        m_FlatColorShader->SetMat4("u_Transform", model);
        glm::vec3 color = m_PointLights[i]->Diffuse;
        m_FlatColorShader->SetFloat4("u_Color", { color.x, color.y, color.z, 1.0 });
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RawOpenGLSandbox::OnImGuiRender()
{
}

void RawOpenGLSandbox::OnEvent(GLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(RawOpenGLSandbox::OnWindowResized));
}

void RawOpenGLSandbox::InitLights()
{
    m_DirectionalLight = std::make_unique<DirectionalLight>(glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.05f, 0.05f, 0.1f),
        glm::vec3(0.2f, 0.2f, 0.7),
        glm::vec3(0.7f, 0.7f, 0.7f));

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    glm::vec3 pointLightColors[] = {
        glm::vec3(0.2f, 0.2f, 0.6f),
        glm::vec3(0.3f, 0.3f, 0.7f),
        glm::vec3(0.0f, 0.0f, 0.3f),
        glm::vec3(0.4f, 0.4f, 0.4f)
    };

    m_PointLights = {
        std::make_unique<PointLight>(glm::vec3(0.7f,  0.2f,  2.0f),
        pointLightColors[0] * glm::vec3(0.1f), pointLightColors[0], pointLightColors[0], constant, linear, quadratic),
        std::make_unique<PointLight>(glm::vec3(2.3f, -3.3f, -4.0f),
        pointLightColors[1] * glm::vec3(0.1f), pointLightColors[1], pointLightColors[1], constant, linear, quadratic),
        std::make_unique<PointLight>(glm::vec3(-4.0f,  2.0f, -12.0f),
        pointLightColors[2] * glm::vec3(0.1f), pointLightColors[2], pointLightColors[2], constant, linear, quadratic),
        std::make_unique<PointLight>(glm::vec3(0.0f,  0.0f, -3.0f),
        pointLightColors[3] * glm::vec3(0.1f), pointLightColors[3], pointLightColors[3], constant, linear, quadratic),
    };

    m_FlashLight = std::make_unique<SpotLight>(glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f, 0.09f, 0.032f, glm::cos(glm::radians(10.0f)), glm::cos(glm::radians(15.0f)));
}

void RawOpenGLSandbox::InitCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

void RawOpenGLSandbox::GenerateTexture2D(const std::string& filepath, uint32_t* texture)
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

bool RawOpenGLSandbox::OnWindowResized(GLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}