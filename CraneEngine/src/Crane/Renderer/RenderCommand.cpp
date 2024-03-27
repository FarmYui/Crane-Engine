#include "crpch.h"
#include "RenderCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Crane
{
	
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}