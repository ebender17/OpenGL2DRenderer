#include "glpch.h"
#include "GLCore/Core/Application.h"

#include "GLCore/Core/Log.h"
#include "GLCore/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace GLCore {

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, uint32_t width, uint32_t height)
    {
        GLCORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create({ name, width, height });
        m_Window->SetEventCallback(GLCORE_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);

                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(GLCORE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(Application::OnWindowResize));

        // iterate backwards to handle events
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            if (event.Handled)
                break;
            (*--it)->OnEvent(event);
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
        return false;
    }

}