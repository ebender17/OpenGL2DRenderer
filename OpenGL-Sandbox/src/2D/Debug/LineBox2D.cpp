#include "LineBox2D.h"

#include <Glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LineBox2D::LineBox2D()
{
    std::string vertexSrc = R"(
        #version 460 core
            
        layout (location = 0) in vec3 a_Position;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Model;

        void main()
        {
            gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
        }
    )";

    std::string fragmentSrc = R"(
        #version 460 core
            
        layout(location = 0) out vec4 color;

        uniform vec3 u_Color;

        void main()
        {
            color = vec4(u_Color, 1.0);
        }
    )";

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom-left
        0.5f, -0.5f, 0.0f,  // Bottom-right
        0.5f,  0.5f, 0.0f,  // Top-right
        -0.5f,  0.5f, 0.0f, // Top-left
        -0.5f, -0.5f, 0.0f  // Close the loop
    };

    m_VertexPosShader = GLCore::Shader::Create("Line", vertexSrc, fragmentSrc);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 5 * 3 * sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

LineBox2D::~LineBox2D()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void LineBox2D::DrawCollider(const GameObject2D& obj, const Camera2DBaseController& cameraController)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), obj.GetPosition())
        * glm::scale(glm::mat4(1.0f), { obj.GetRect().SpriteSize.x, obj.GetRect().SpriteSize.y, 1.0f});
    m_VertexPosShader->Bind();
    m_VertexPosShader->SetMat4("u_ViewProjection", cameraController.GetCamera().GetViewProjectionMatrix());
    m_VertexPosShader->SetMat4("u_Model", model);
    m_VertexPosShader->SetFloat3("u_Color", m_BoxColor);

    glBindVertexArray(m_VAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    glBindVertexArray(0);
}
