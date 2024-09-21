#include "glpch.h"
#include "GLCore/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GLCore {

    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}