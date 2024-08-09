#include "Application.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Core/Log.h"

namespace GLCore {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1200, 720);
		GLCORE_LOG_TRACE(e.ToString()); // TODO : figure out why this do not work without .ToString()

		while (true);
	}

}