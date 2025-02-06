#pragma once

#ifdef GLCORE_PLATFORM_WINDOWS

extern GLCore::Application* GLCore::CreateApplication();

int main(int argc, char** argv)
{
    GLCore::Log::Init();
    auto app = GLCore::CreateApplication();
    app->Run();
    delete app;
}

#endif