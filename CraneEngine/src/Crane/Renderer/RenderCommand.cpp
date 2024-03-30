#include "crpch.h"
#include "RenderCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Crane
{
	
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}