#pragma once

#include "Crane/Window.h"
#include "Crane/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Crane
{
	class CRANE_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//window attribs
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		virtual void SetVSync(bool On) override;
		virtual bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}