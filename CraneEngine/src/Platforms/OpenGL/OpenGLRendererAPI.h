#pragma once

#include "Crane/Renderer/RendererAPI.h"
#include "Crane/Renderer/VertexArray.h"

namespace Crane
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;
		virtual ~OpenGLRendererAPI() = default;
		
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& VertexArray) override;
	};
}