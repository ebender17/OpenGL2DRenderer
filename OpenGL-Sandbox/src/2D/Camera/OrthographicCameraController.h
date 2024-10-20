#pragma once

#include <GLCore.h>

#include "Camera2DBaseController.h"

// TODO : rename & test
class OrthographicCameraController : public Camera2DBaseController
{
public:
    OrthographicCameraController(float aspectRatio, bool rotation = false, bool zoom = false, float nearPlane = -1.0f, float farPlane = 1.0f);

    virtual void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnEvent(GLCore::Event& event) override;
private:

    bool OnMouseScrolled(GLCore::MouseScrolledEvent& event);
private:
    bool m_Rotation;
    bool m_Zoom;
};

