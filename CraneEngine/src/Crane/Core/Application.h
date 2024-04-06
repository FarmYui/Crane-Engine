#pragma once

#include "Crane/Core/Core.h"

#include "Crane/Core/Window.h"
#include "Crane/Core/LayerStack.h"
#include "Crane/ImGui/ImGuiLayer.h"

#include "Crane/Events/Event.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"

#include "Crane/Renderer/OrthographicCamera.h"


namespace Crane 
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Close();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() const { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		static Application* s_Instance;
	private:
		std::unique_ptr<Window> m_Window;
		
		bool m_Running = true;

		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		bool m_Minimized = false;

		float m_LastFrameTime = 0;
		
	};

	//To define in client
	Application* CreateApplication();
}