#include "glpch.h"
#include "Application.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Core/Log.h"

#include <GLFW/glfw3.h>

namespace GLCore {

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		m_Window = std::unique_ptr<Window>(Window::Create({ name, width, height }));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}