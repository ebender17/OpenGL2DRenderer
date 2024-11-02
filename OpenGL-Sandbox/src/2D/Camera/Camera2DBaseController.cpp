#include "Camera2DBaseController.h"

using namespace GLCore;

Camera2DBaseController::Camera2DBaseController(float aspectRatio, float nearPlane, float farPlane)
    : m_AspectRatio(aspectRatio),
    m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
    m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, nearPlane, farPlane)
{
    CalculateView();
}

void Camera2DBaseController::OnUpdate(Timestep timestep)
{
    PROFILE_FUNCTION();
}

void Camera2DBaseController::OnEvent(Event& event)
{
    PROFILE_FUNCTION();

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(Camera2DBaseController::OnWindowResized));
}

void Camera2DBaseController::CalculateView()
{
    m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
    m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
}

bool Camera2DBaseController::OnWindowResized(WindowResizeEvent& event)
{
    PROFILE_FUNCTION();

    m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
    CalculateView();
    return false;
}