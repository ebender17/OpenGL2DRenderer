#pragma once

#include "Core.h"

namespace GLCore {

	class GLCORE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be define in client
	Application* CreateApplication();

}

