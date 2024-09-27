#include "glpch.h"
#include "Platform/OpenGL/Renderer/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace GLCore {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        :m_WindowHandle(windowHandle)
    {
        GLCORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        GLCORE_ASSERT(status, "Failed to initialize Glad!");

        std::string Vendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        std::string Renderer(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        std::string Version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        LOG_INFO("OpenGL Info:");
        LOG_INFO("  Vendor: {0}", Vendor);
        LOG_INFO("  Renderer: {0}", Renderer);
        LOG_INFO("  Version: {0}", Version);

        GLCORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}