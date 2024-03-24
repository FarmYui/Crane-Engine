#pragma once

#include "VertexArray.h"

namespace Crane
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& VertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	
	private:
		static API s_API;
	};
}