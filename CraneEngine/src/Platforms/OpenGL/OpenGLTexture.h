#pragma once

#include "Crane/Renderer/Texture.h"

namespace Crane
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t m_RendererID;

		unsigned char* m_Buffer;
		int m_Width;
		int m_Height;
		int m_NrChannels;
	};
}