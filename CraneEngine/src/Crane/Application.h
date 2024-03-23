#pragma once

#include "Core.h"

#include "Window.h"
#include "Crane/LayerStack.h"
#include "Crane/ImGui/ImGuiLayer.h"

#include "Crane/Events/Event.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"

#include "Crane/Renderer/Shader.h"
#include "Crane/Renderer/Buffer.h"

namespace Crane 
{
	class CRANE_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

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

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};

	//To define in client
	Application* CreateApplication();
}