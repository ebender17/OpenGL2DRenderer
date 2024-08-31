#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class SandboxLayer : public GLCore::Layer
{
public:
    SandboxLayer();
    virtual ~SandboxLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(GLCore::Event& event) override;
    virtual void OnUpdate() override; // TODO - add timestep
    virtual void OnImGuiRender() override;
private:
};

