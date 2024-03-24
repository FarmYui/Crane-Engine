#pragma once

#include "RendererAPI.h"
#include "RendererCommand.h"

namespace Crane
{
	class Renderer
	{
	public:
		static void BeginScene() {};
		static void EndScene() {};

		static void Submit(const std::shared_ptr<VertexArray>& VertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}