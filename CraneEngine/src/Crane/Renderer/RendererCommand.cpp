#include "crpch.h"
#include "RendererCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Crane
{
	
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

}