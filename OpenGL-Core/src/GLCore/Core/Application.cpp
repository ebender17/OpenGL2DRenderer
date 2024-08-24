#include "glpch.h"
#include "Application.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Core/Log.h"

#include <glad/glad.h>

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

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); // call OnWindowClose on WindowCloseEvents
        GLCORE_LOG_INFO("{0}", event.ToString());

        // iterate backwards to handle events
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;
        return true;
    }

}