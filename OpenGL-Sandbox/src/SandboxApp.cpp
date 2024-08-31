#include <GLCore.h>
#include <GLCore/Core/EntryPoint.h>

#include "SandboxLayer.h"

class Sandbox : public GLCore::Application
{
public: 
    Sandbox()
    {
        PushLayer(new SandboxLayer());
    }

    ~Sandbox()
    {

    }
};

GLCore::Application* GLCore::CreateApplication()
{
    return new Sandbox();
}