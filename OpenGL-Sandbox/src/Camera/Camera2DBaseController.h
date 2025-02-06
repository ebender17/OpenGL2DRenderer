#pragma once

#include <GLCore.h>

class Camera2DBaseController
{
public:
    struct OrthographicCameraBounds
    {
        float Left;
        float Right;
        float Bottom;
        float Top;

        float GetWidth() { return Right - Left; }
        float GetHeight() { return Top - Bottom; }
    };

public:
    Camera2DBaseController(float aspectRatio, float nearPlane = -1.0f, float farPlane = 1.0f);

    virtual void OnUpdate(GLCore::Timestep timestep);
    virtual void OnEvent(GLCore::Event& event);

    GLCore::OrthographicCamera& GetCamera() { return m_Camera; }
    const GLCore::OrthographicCamera& GetCamera() const { return m_Camera; }

    float GetZoomLevel() const { return m_ZoomLevel; }
    void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }

    const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
protected:
    void CalculateView();
private:
    bool OnWindowResized(GLCore::WindowResizeEvent& event);
protected:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    OrthographicCameraBounds m_Bounds;
    GLCore::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 0.0f; // In degrees, anti-clockwise direction
    float m_CameraTranslationSpeed = 0.5f;
    float m_CameraRotationSpeed = 180.0f;
};

