#include <GLCore.h>
#include <GLCore/Core/EntryPoint.h>

#include "SandboxLayer.h"
#include "Sandbox2D.h"
#include "RawOpenGLSandbox.h"

class Sandbox : public GLCore::Application
{
public: 
    Sandbox()
    {
        // PushLayer(new SandboxLayer());
        // PushLayer(new Sandbox2D());
        PushLayer(new RawOpenGLSandbox());
    }

    ~Sandbox()
    {

    }
};

GLCore::Application* GLCore::CreateApplication()
{
    return new Sandbox();
}