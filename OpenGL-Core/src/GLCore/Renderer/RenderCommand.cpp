#include "glpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GLCore {

    std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}