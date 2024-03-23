#pragma once

#include "Crane/Renderer/Buffer.h"

namespace Crane
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
	
		virtual void Bind() const;
		virtual void Unbind() const;
	private:
		uint32_t m_RendererID;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);

		virtual void Bind() const;
		virtual void Unbind() const;

		inline virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}


