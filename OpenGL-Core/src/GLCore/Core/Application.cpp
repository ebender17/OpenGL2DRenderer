#include "glpch.h"
#include "Application.h"

#include "Log.h"
#include "Input.h"
#include "GLCore/Renderer/Renderer.h"

#include <glfw/glfw3.h>

namespace GLCore {

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, uint32_t width, uint32_t height)
    {
        GLCORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create({ name, width, height }));
        m_Window->SetEventCallback(GLCORE_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(GLCORE_BIND_EVENT_FN(Application::OnWindowClose));

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