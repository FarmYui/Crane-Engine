#include "crpch.h"

#include "Crane/Core/Core.h"

#include "WindowsWindow.h"

#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"

#include "Platforms/OpenGL/OpenGLContext.h"	

namespace Crane
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		CR_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
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
		
		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			CR_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		
	
		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();
		
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
				KeyReleasedEvent e(key);
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
		
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
		{
			// get user ptr
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// create the event
			KeyTypedEvent e(codepoint);

			// call event callback
			data.EventCallback(e);
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
			MouseScrolledEvent e((float)xoffset, (float)yoffset);
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
		--s_GLFWWindowCount;


		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
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