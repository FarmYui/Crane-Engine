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

		virtual void Init() override;
		virtual void ResizeScreen(float width, float height) override;
		
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& VertexArray) override;
	};
}