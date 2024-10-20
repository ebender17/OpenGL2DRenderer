#pragma once

#include <GLCore.h>

#include "Camera2DBaseController.h"

class InputCamera2DController : public Camera2DBaseController
{
public:
    InputCamera2DController(float aspectRatio, bool rotation = false, bool zoom = false, float nearPlane = -1.0f, float farPlane = 1.0f);

    virtual void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnEvent(GLCore::Event& event) override;
private:

    bool OnMouseScrolled(GLCore::MouseScrolledEvent& event);
private:
    bool m_Rotation;
    bool m_Zoom;
};

