#include <GLCore.h>
#include <GLCore/Core/EntryPoint.h>

#include "SandboxLayer.h"
#include "Sandbox2D.h"

class Sandbox : public GLCore::Application
{
public: 
    Sandbox()
    {
        // PushLayer(new SandboxLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {

    }
};

GLCore::Application* GLCore::CreateApplication()
{
    return new Sandbox();
}