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
		LOG_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(GLCore::Event& event) override
	{
		LOG_INFO("{0}", event.ToString());
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