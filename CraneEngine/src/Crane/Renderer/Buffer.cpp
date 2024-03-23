#include "crpch.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLBuffer.h"

namespace Crane
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			CR_CORE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);

		default:
			CR_CORE_ASSERT(false, "RendererAPI unknown!");
			return nullptr;
		}
	}


	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			CR_CORE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);

		default:
			CR_CORE_ASSERT(false, "RendererAPI unknown!");
			return nullptr;
		}
	}
}