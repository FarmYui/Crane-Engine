#include "crpch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLTexture.h"

namespace Crane
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, unsigned int channels)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, channels);

		default:
			CR_CORE_ASSERT(false, "RendererAPI unknown!");
			return nullptr;
		}
	}

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);

		default:
			CR_CORE_ASSERT(false, "RendererAPI unknown!");
			return nullptr;
		}
    }
}