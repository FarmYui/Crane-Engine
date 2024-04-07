#include "crpch.h"
#include "Framebuffer.h"

#include "Crane/Renderer/Renderer.h"
#include "Platforms/OpenGL/OpenGLFramebuffer.h"


namespace Crane
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(specification);

		default:
			CR_CORE_ASSERT(false, "RendererAPI unknown!");
			return nullptr;
		}	
    }
}
