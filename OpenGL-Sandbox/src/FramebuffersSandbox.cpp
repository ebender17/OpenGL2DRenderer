#include "FramebuffersSandbox.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"
#include "Platform/OpenGL/Renderer/OpenGLShader.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <stb_image/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format> // TODO : delete in both projects?

using namespace GLCore;

FramebuffersSandbox::FramebuffersSandbox()
    : Layer("FramebuffersSandbox")
{
}

void FramebuffersSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK); // default
    glFrontFace(GL_CW); // default is GL_CCW

    // vertex position, tex coords
    float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
    };
    // positions, tex coords
    float planeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    };
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

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

    glGenVertexArrays(1, &m_PlaneVAO);
    glBindVertexArray(m_PlaneVAO);
    glGenBuffers(1, &m_PlaneVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);
    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    // Load and create textures
    GenerateTexture2D("assets/textures/wooden-box-diffuse.png", &m_WoodenBoxTexture, GL_REPEAT);
    GenerateTexture2D("assets/textures/metal.png", &m_MetalTexture, GL_REPEAT);

    // Shaders
    m_Shader = std::make_unique<OpenGLShader>("assets/shaders/FlatTexture.glsl");
    m_Shader->Bind();
    m_Shader->SetInt("u_Texture", 0);

    m_FullScreenQuadShader = std::make_unique<OpenGLShader>("assets/shaders/PostProcess.glsl");
    m_FullScreenQuadShader->Bind();
    m_FullScreenQuadShader->SetInt("u_ScreenTexture", 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Generate framebuffer
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    GenerateTextureFramebuffer(&m_FramebufferTexture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FramebufferTexture, 0);
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    // TODO : replace with window width and height
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280.0f, 720.0f);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Framebuffer is not complete!");
    }
    // bind default framebuffer (main window)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebuffersSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteBuffers(1, &m_CubeVBO);
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteBuffers(1, &m_QuadVBO);
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteRenderbuffers(1, &m_RBO);
}

void FramebuffersSandbox::OnUpdate(GLCore::Timestep timestep)
{
    // Update
    m_Camera->OnUpdate(timestep);

    // Render
    // first pass
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawScene();

    // second pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_FullScreenQuadShader->Bind();
    m_FullScreenQuadShader->SetInt("u_PostProcessOption", (int)m_PostProcessOption);
    glBindVertexArray(m_QuadVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FramebufferTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FramebuffersSandbox::OnImGuiRender()
{
    static int currentOption = 0;
    ImGui::Begin("Post Process Settings");
    if (ImGui::RadioButton("Inversion", currentOption == 0))
        currentOption = 0;
    if (ImGui::RadioButton("Grayscale", currentOption == 1))
        currentOption = 1;
    if (ImGui::RadioButton("Sharpen", currentOption == 2))
        currentOption = 2;
    if (ImGui::RadioButton("Blur", currentOption == 3))
        currentOption = 3;
    
    m_PostProcessOption = PostProcesOptions(currentOption);
    ImGui::End();
}

void FramebuffersSandbox::OnEvent(GLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(FramebuffersSandbox::OnWindowResized));
}

void FramebuffersSandbox::InitCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

// TODO : make utility function & replace where duplicated in sandboxes
void FramebuffersSandbox::GenerateTexture2D(const std::string& filepath, uint32_t* texture, uint32_t wrapOption)
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

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void FramebuffersSandbox::GenerateTextureFramebuffer(uint32_t* texture)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    // TODO : replace with window width and height
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280.0f, 720.0f, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool FramebuffersSandbox::OnWindowResized(GLCore::WindowResizeEvent& event)
{
    m_WindowWidth = event.GetWidth();
    m_WindowHeight = event.GetHeight();

    m_Camera->SetAspectRatio(m_WindowWidth, m_WindowHeight);
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);
    return false;
}

void FramebuffersSandbox::DrawScene()
{
    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glm::vec3 camPos = m_Camera->GetPosition();
    m_Shader->Bind();
    m_Shader->SetMat4("u_ViewProjection", viewProjectionMatrix);

    // Floor
    glDisable(GL_CULL_FACE);
    glBindVertexArray(m_PlaneVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_MetalTexture);
    glm::mat4 modelFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 0.0f));
    modelFloor = glm::rotate(modelFloor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelFloor = glm::scale(modelFloor, glm::vec3(7.0f, 7.0f, 1.0f));
    m_Shader->SetMat4("u_Model", modelFloor);
    m_Shader->SetFloat("u_TilingFactor", 7.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw boxes
    glEnable(GL_CULL_FACE);
    glBindVertexArray(m_CubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_WoodenBoxTexture);
    m_Shader->SetFloat("u_TilingFactor", 1.0f);
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_Shader->SetMat4("u_Model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f));
    m_Shader->SetMat4("u_Model", model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}