#include "TargetCamera2DController.h"

TargetCamera2DController::TargetCamera2DController(float aspectRatio, float nearPlane, float farPlane)
    : Camera2DBaseController(aspectRatio, nearPlane, farPlane), m_Target(0.0f)
{
}

void TargetCamera2DController::OnUpdate(GLCore::Timestep timestep)
{
    Camera2DBaseController::OnUpdate(timestep);

    float width = m_Bounds.GetWidth();
    float height = m_Bounds.GetHeight();
    // TODO : left off here
    // m_Target.x = glm::clamp(m_Target.x, m_MapBounds[0] + (width / 2.0f), (m_MapBounds[1] / 2.0f));
    // m_Target.y = glm::clamp(m_Target.y, m_MapBounds[2] + (height / 2.0f), (m_MapBounds[3] / 2.0f));

    m_Camera.SetPosition({ m_Target.x, m_Target.y, 0.0f });
}

void TargetCamera2DController::OnEvent(GLCore::Event& event)
{
    Camera2DBaseController::OnEvent(event);

    // TODO : do i need to create this override if not using?
}
