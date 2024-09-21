#include "glpch.h"
#include "WindowsWindow.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/KeyEvent.h"
#include "GLCore/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace GLCore {

    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (s_GLFWWindowCount == 0)
        {
            int success = glfwInit();
            GLCORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;

        m_Context = CreateScope<OpenGLContext>(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent event(static_cast<KeyCode>(key));
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        // TODO : GLFW does not provide use with a repeat count although the Win32 API can.
                        KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                        data.EventCallback(event);
                        break;
                    }
                }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                        data.EventCallback(event);
                        break;
                    }
                }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xoffset, (float)yoffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xpos, (float)ypos);
            data.EventCallback(event);
        });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;

        if (--s_GLFWWindowCount == 0)
        {
            LOG_INFO("Terminating GLFW");
            glfwTerminate();
        }
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

}