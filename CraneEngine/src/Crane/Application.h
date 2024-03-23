#pragma once

#include "Core.h"

#include "Window.h"
#include "Crane/LayerStack.h"
#include "Crane/ImGui/ImGuiLayer.h"

#include "Crane/Events/Event.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"

#include "Crane/Renderer/Shader.h"

namespace Crane 
{
	class CRANE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() const { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		static Application* s_Instance;
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;

		std::unique_ptr<Shader> m_Shader;
	};

	//To define in client
	Application* CreateApplication();
}