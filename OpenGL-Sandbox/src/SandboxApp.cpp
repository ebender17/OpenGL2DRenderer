#include <GLCore.h>

class Sandbox : public GLCore::Application
{
public: 
	Sandbox()
	{
		int a = 5;
		LOG_INFO("Hello from Sandbox! Var={0}", a);
	}

	~Sandbox()
	{

	}
};

GLCore::Application* GLCore::CreateApplication()
{
	return new Sandbox();
}