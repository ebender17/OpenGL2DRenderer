#include <GLCore.h>
#include <GLCore/Core/EntryPoint.h>

#include "SandboxLayer.h"
#include "Sandbox2D.h"
#include "Basic3DLightingSandbox.h"
#include "OpenGLSandbox.h"

class Sandbox : public GLCore::Application
{
public: 
    Sandbox()
    {
        // TODO : Create a wrapper Sandbox GUI to select which Sandbox to view
        // PushLayer(new SandboxLayer());
        PushLayer(new Sandbox2D());
        // PushLayer(new Basic3DLightingSandbox());
        // PushLayer(new OpenGLSandbox());
    }

    ~Sandbox()
    {

    }
};

GLCore::Application* GLCore::CreateApplication()
{
    return new Sandbox();
}