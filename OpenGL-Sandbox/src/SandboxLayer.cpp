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

void SandboxLayer::OnUpdate() // TODO - timestep
{
    if (Input::IsKeyPressed(GLCORE_KEY_TAB))
        LOG_TRACE("Tab key is pressed!");

    // auto [x, y] = Input::GetMousePosition();
    // LOG_INFO("{0}, {1}", x, y);
}

void SandboxLayer::OnImGuiRender()
{
}