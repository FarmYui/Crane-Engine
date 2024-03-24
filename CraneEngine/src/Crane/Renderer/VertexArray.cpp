#include "crpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace Crane
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();

		default:
			CR_CORE_ASSERT(false, "RendererAPI unknown!");
			return nullptr;
		}
	}
}