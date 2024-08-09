#pragma once

#include "Core.h"
#include "GLCore/Events/Event.h"

namespace GLCore {

	class GLCORE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client
	Application* CreateApplication();

}

