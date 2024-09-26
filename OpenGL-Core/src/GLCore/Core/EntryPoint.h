#pragma once

#ifdef GLCORE_PLATFORM_WINDOWS

extern GLCore::Application* GLCore::CreateApplication();

int main(int argc, char** argv)
{
    GLCore::Log::Init();

    PROFILE_BEGIN_SESSION("Startup", "Profile_Startup.json");
    auto app = GLCore::CreateApplication();
    PROFILE_END_SESSION();

    PROFILE_BEGIN_SESSION("Runtime", "Profile_Runtime.json");
    app->Run();
    PROFILE_END_SESSION();

    PROFILE_BEGIN_SESSION("Shutdown", "Profile_Shutdown.json");
    delete app;
    PROFILE_END_SESSION();
}

#endif