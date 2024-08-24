#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "GLCore/Events/Event.h"
#include "GLCore/Events/ApplicationEvent.h"

namespace GLCore {

    class GLCORE_API Application
    {
    public:
        Application(const std::string& name = "OpenGL Sandbox", uint32_t width = 1280, uint32_t height = 720);
        virtual ~Application();

        void Run();

        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
    private:
        bool OnWindowClose(WindowCloseEvent& event);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    };

    // To be defined in client
    Application* CreateApplication();

}