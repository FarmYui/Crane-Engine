#include "crpch.h"

#include "Crane/Core.h"

#include "WindowsWindow.h"

#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"

#include <glad/glad.h>

namespace Crane
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		CR_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title  = props.Title;
		m_Data.Width  = props.Width;
		m_Data.Height = props.Height;

		CR_CORE_INFO("Creating window '{0}' ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);
		
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			CR_CORE_ASSERT(success, "Could not initialize GLFW");

			glfwSetErrorCallback(GLFWErrorCallback);
			
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CR_CORE_ASSERT(status, "Could not initialize Glad");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{ 
			// get user ptr
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			// update data
			data.Width = width;
			data.Height = height;

			// dispatch event
			WindowResizeEvent e(width, height);
			data.EventCallback(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			// get user ptr
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// create event
			WindowCloseEvent e;
			data.EventCallback(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// get user ptr
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// create event
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent e(key, 0);
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyRelasedEvent e(key);
				data.EventCallback(e);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent e(key, 1);
				data.EventCallback(e);
				break;
			}
			default:
				break;
			}
			
		});
		
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			// get user ptr
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent e(button);
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent e(button);
				data.EventCallback(e);
				break;
			}
			default:
				break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			//get user ptr
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// create event
			MouseScolledEvent e((float)xoffset, (float)yoffset);
			data.EventCallback(e);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			// get user ptr 
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// create event
			MouseMovedEvent e((float)xpos, (float)ypos);
			data.EventCallback(e);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool On)
	{
		if (On)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = On;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}