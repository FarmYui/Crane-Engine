#pragma once

#include "RendererAPI.h"

#include <glm/glm.hpp>

namespace Crane
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void ResizeScreen(float width, float height)
		{
			s_RendererAPI->ResizeScreen(width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color.r, color.g, color.b, color.a);
		}

		inline static void SetClearColor(float r, float g, float b, float a)
		{
			s_RendererAPI->SetClearColor(r, g, b, a);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& VertexArray)
		{
			s_RendererAPI->DrawIndexed(VertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}