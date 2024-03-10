#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"

#include "Window.h"

namespace Crane 
{
	class CRANE_API Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);


		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//To define in client
	Application* CreateApplication();
}