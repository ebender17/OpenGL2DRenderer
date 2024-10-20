#pragma once

#include <GLCore.h>

#include "Camera2DBaseController.h"

class TargetCameraController : public Camera2DBaseController
{
public:
    TargetCameraController(float aspectRatio, float nearPlane = -1.0f, float farPlane = 1.0f);

    void OnUpdate(GLCore::Timestep timestep);
    virtual void OnEvent(GLCore::Event& event) override;

    void SetBounds(const float values[4])
    {
        for (size_t i = 0; i < 4; i++)
        {
            m_MapBounds[i] = values[i];
        }
    }

    void SetTarget(const glm::vec2& target) { m_Target = target; }
private:
    glm::vec2 m_Target;
    float m_MapBounds[4]; // 0 = minX, 1 = maxX, 2 = minY, 3 = maxY
};

