#include "crpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Crane
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CR_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init() 
	{
		CR_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CR_CORE_ASSERT(status, "Could not initialize Glad");
		
		CR_CORE_INFO("OpenGL Info:");
		CR_CORE_INFO("    Vendor   : {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR))  );
		CR_CORE_INFO("    Renderer : {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		CR_CORE_INFO("    Version  : {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)) );

		CR_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		CR_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}

	/*std::string OpenGLContext::GetInfo() const 
	{
		std::stringstream ss;
		ss << "OpenGL Info:";
		ss << "\nVendor   : " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		ss << "\nRenderer : " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		ss << "\nVersion  : " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
		return ss.str();
	}*/
}