#include <GLCore.h>

class ExampleLayer : public GLCore::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        if (GLCore::Input::IsKeyPressed(GLCORE_KEY_TAB))
            LOG_TRACE("Tab key is pressed!");
    }

    void OnEvent(GLCore::Event& event) override
    {
        if (event.GetEventType() == GLCore::EventType::KeyPressed)
        {
            GLCore::KeyPressedEvent& e = (GLCore::KeyPressedEvent&)event;
            // keycodes line up with ASCII so can cast to char to print char pressed
            LOG_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
};

class Sandbox : public GLCore::Application
{
public: 
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

GLCore::Application* GLCore::CreateApplication()
{
    return new Sandbox();
}