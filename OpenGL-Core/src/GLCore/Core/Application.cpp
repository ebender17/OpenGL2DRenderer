#include "glpch.h"
#include "Application.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Core/Log.h"

#include <GLFW/glfw3.h>

namespace GLCore {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		m_Window = std::unique_ptr<Window>(Window::Create({ name, width, height }));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent)); // sets up OnEvent to be called when OnEventCallback is called
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

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); // call OnWindowClose on WindowCloseEvents
		GLCORE_LOG_INFO("{0}", event.ToString());
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

}