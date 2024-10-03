#include "glpch.h"
#include "GLCore/Renderer/OrthographicCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"

namespace GLCore {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation, bool zoom, float nearPlane, float farPlane)
        : m_AspectRatio(aspectRatio),
        m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
        m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, nearPlane, farPlane),
        m_Rotation(rotation), m_Zoom(zoom)
    {
        CalculateView();
    }

    void OrthographicCameraController::OnUpdate(Timestep timestep)
    {
        PROFILE_FUNCTION();

        if (Input::IsKeyPressed(Key::A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }
        if (Input::IsKeyPressed(Key::D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }
        if (Input::IsKeyPressed(Key::W))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }
        if (Input::IsKeyPressed(Key::S))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }

        m_Camera.SetPosition(m_CameraPosition);
        m_CameraTranslationSpeed = m_ZoomLevel;

        if (!m_Rotation) { return; }

        if (Input::IsKeyPressed(Key::Q))
            m_CameraRotation += m_CameraRotationSpeed * timestep;
        if (Input::IsKeyPressed(Key::E))
            m_CameraRotation -= m_CameraRotationSpeed * timestep;

        if (m_CameraRotation > 180.0f)
            m_CameraRotation -= 360.0f;
        else if (m_CameraRotation <= -180.0f)
            m_CameraRotation += 360.0f;

        m_Camera.SetRotation(m_CameraRotation);
    }

    void OrthographicCameraController::OnEvent(Event& event)
    {
        PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::CalculateView()
    {
        m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
        m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
    {
        PROFILE_FUNCTION();

        if (!m_Zoom) { return false; }

        m_ZoomLevel -= event.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        CalculateView();
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
    {
        PROFILE_FUNCTION();

        m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
        CalculateView();
        return false;
    }

}