#pragma once

#ifdef GLCORE_PLATFORM_WINDOWS

extern GLCore::Application* GLCore::CreateApplication();

int main(int argc, char** argv)
{
	GLCore::Log::Init();
	GLCORE_LOG_WARN("Initialized Log!");
	int a = 5;
	LOG_INFO("Hello! Var={0}", a);

	auto app = GLCore::CreateApplication();
	app->Run();
	delete app;
}

#endif