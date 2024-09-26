#include "glpch.h"
#include "GLCore/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GLCore {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, nearPlane, farPlane)), m_ViewMatrix(1.0f)
    {
        PROFILE_FUNCTION();

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane)
    {
        PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        PROFILE_FUNCTION();

        glm::mat4 identity = glm::mat4(1.0f);
        glm::mat4 transform = glm::translate(identity, m_Position) *
            glm::rotate(identity, glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}