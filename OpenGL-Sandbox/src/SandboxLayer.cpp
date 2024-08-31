#include "SandboxLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnEvent(Event& event)
{
    if (event.GetEventType() == EventType::KeyPressed)
    {
        KeyPressedEvent& e = (KeyPressedEvent&)event;
        // keycodes line up with ASCII so can cast to char to print char pressed
        LOG_TRACE("{0}", (char)e.GetKeyCode());
    }
}

void SandboxLayer::OnUpdate(Timestep timestep)
{
    glClearColor(0.5, 0.5, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (Input::IsKeyPressed(GLCORE_KEY_TAB))
        LOG_TRACE("Tab key is pressed!");

    LOG_INFO("Timestep in seconds: {0}", timestep.GetSeconds());
    // auto [x, y] = Input::GetMousePosition();
    // LOG_INFO("{0}, {1}", x, y);
}

void SandboxLayer::OnImGuiRender()
{
}