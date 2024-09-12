#include "glpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GLCore {

    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}