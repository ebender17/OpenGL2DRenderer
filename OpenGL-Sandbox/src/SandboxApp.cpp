#include <GLCore.h>

class Sandbox : public GLCore::Application
{
public: 
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

GLCore::Application* GLCore::CreateApplication()
{
	return new Sandbox();
}