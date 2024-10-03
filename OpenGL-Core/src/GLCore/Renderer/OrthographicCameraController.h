#pragma once

#include "GLCore/Renderer/OrthographicCamera.h"
#include "GLCore/Core/Timestep.h"
#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"

namespace GLCore {

    struct OrthographicCameraBounds
    {
        float Left;
        float Right;
        float Bottom;
        float Top;

        float GetWidth() { return Right - Left; }
        float GetHeight() { return Top - Bottom; }
    };

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false, bool zoom = false, float nearPlane = -1.0f, float farPlane = 1.0f);

        void OnUpdate(Timestep timestep);
        void OnEvent(Event& event);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

        float GetZoomLevel() const { return m_ZoomLevel; }
        void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }

        const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
    private:
        void CalculateView();

        bool OnMouseScrolled(MouseScrolledEvent& event);
        bool OnWindowResized(WindowResizeEvent& event);
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCameraBounds m_Bounds;
        OrthographicCamera m_Camera;

        bool m_Rotation;
        bool m_Zoom;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f; // In degrees, anti-clockwise direction
        float m_CameraTranslationSpeed = 0.5f;
        float m_CameraRotationSpeed = 180.0f;
    };

}

