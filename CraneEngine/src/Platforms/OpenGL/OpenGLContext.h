#pragma once
#include "Crane/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Crane 
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext() {};

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual std::string GetInfo() const override;
	private:
		GLFWwindow* m_WindowHandle;

	};
}