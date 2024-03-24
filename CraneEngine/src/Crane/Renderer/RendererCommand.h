#pragma once

#include "RendererAPI.h"

namespace Crane
{
	class RendererCommand
	{
	public:
		inline static void SetClearColor(float r, float g, float b, float a)
		{
			s_RendererAPI->SetClearColor(r, g, b, a);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& VertexArray)
		{
			s_RendererAPI->DrawIndexed(VertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}