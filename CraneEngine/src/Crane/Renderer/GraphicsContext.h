#pragma once

namespace Crane
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		virtual std::string GetInfo() const = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
}