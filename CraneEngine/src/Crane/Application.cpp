#include "crpch.h"

#include "Application.h"
#include "Platforms/Windows/WindowsInput.h"

#include "Renderer/Renderer.h"

#include <glad/glad.h>


namespace Crane
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		CR_CORE_ASSERT(!s_Instance, "Application alredy exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(CR_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// // // //// // // //// // // //// // // //// // // //

		float vertices[] = {
			// quad
			-2.0f, -0.04f, 0.0f, 1.0f, 1.0f, 1.0f,
			 2.0f, -0.04f, 0.0f, 1.0f, 1.0f, 1.0f,
			 2.0f,  0.04f, 0.0f, 1.0f, 1.0f, 1.0f,
			-2.0f,  0.04f, 0.0f, 1.0f, 1.0f, 1.0f,

			// triangle
			-0.6f, -0.5f, 0.0f, 1.0f, 0.3f, 1.0f,
			 0.6f, -0.5f, 0.0f, 1.0f, 1.0f, 0.6f,
			 0.0f,  0.5f, 0.0f, 0.1f, 1.0f, 1.0f
		};

		uint32_t indices[] = { 
			0,1,2,
			0,2,3,
			4,5,6,
		};

		// create buffers + varray
		m_VertexArray.reset(VertexArray::Create());
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// setting the layout of the vertex buffer
		BufferLayout layout = {
			{ ShaderDataType::Float3 , "a_Position" },
			{ ShaderDataType::Float3 , "a_Color"}
		};
		m_VertexBuffer->SetLayout(layout);
		
		// add both buffers into Vertex Array
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// shaders
		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;

			uniform mat4 u_Model;
			uniform mat4 u_ViewProjection;			

			out vec3 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Model * vec4(a_Position ,1.0f);
			})";

		std::string fragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Color;

			void main()
			{
				color = vec4(v_Color,1.0f);
			})";

		m_Shader = std::make_unique<Shader>(vertexSource, fragmentSource);
	}


	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CR_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		glm::mat4 model(1.0f);

		while (m_Running)
		{
			model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			m_Shader->SetUniformMat4("u_Model", model);

			RendererCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			RendererCommand::Clear();

			//m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			//m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);
			
			Renderer::Submit(m_VertexArray, m_Shader);

			Renderer::EndScene();
			
			// layer update
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			
			// imgui update
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
				
			// window update
			m_Window->OnUpdate();
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}