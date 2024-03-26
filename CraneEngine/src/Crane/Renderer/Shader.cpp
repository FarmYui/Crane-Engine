#include "crpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLShader.h"


namespace Crane
{

	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSource, fragmentSource);

		default:
			CR_CORE_ASSERT(false, "RendererAPI unknown!");
			return nullptr;
		}

	}
	
}