#include <Crane.h>
#include "imgui/imgui.h"

class TriangleTest : public Crane::Layer
{
public:
	TriangleTest()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
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
		m_VertexArray.reset(Crane::VertexArray::Create());
		m_VertexBuffer.reset(Crane::VertexBuffer::Create(vertices, sizeof(vertices)));
		m_IndexBuffer.reset(Crane::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// setting the layout of the vertex buffer
		Crane::BufferLayout layout = {
			{ Crane::ShaderDataType::Float3 , "a_Position" },
			{ Crane::ShaderDataType::Float3 , "a_Color"}
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

		m_Shader = std::make_unique<Crane::Shader>(vertexSource, fragmentSource);
	}

	void OnUpdate(Crane::Timestep ts) override
	{
		m_Timestep = ts;

		if (Crane::Input::IsKeyPressed(CR_KEY_W))
		{
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts.GetSeconds();
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts.GetSeconds();
		}
		if (Crane::Input::IsKeyPressed(CR_KEY_S))
		{
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts.GetSeconds();
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts.GetSeconds();
		}
		if (Crane::Input::IsKeyPressed(CR_KEY_A))
		{
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts.GetSeconds();
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts.GetSeconds();
		}
		if (Crane::Input::IsKeyPressed(CR_KEY_D))
		{
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts.GetSeconds();
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts.GetSeconds();
		}

		if (Crane::Input::IsMouseButtonPressed(CR_MOUSE_BUTTON_3))
		{
			float currentMouseX = Crane::Input::GetMouseX();
			m_CameraRotation += (m_PrevMouseX) ? ((currentMouseX - m_PrevMouseX) * m_CameraRotationSpeed * ts.GetSeconds()) : 0;
			m_PrevMouseX = currentMouseX;
		}
		else
		{
			m_PrevMouseX = 0;
		}
		


		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		glm::mat4 model(1.0f);
		//model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_Shader->SetUniformMat4("u_Model", model);

		Crane::RendererCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Crane::RendererCommand::Clear();
		
		Crane::Renderer::BeginScene(m_Camera);

		Crane::Renderer::Submit(m_VertexArray, m_Shader);

		Crane::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Info");
		ImGui::Text("FPS: %f", 1000.0f / m_Timestep.GetMilliseconds());
		ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
		ImGui::End();
	}

	void OnEvent(Crane::Event& e) override
	{
	}

private:
	float m_PrevMouseX = 0;

	float m_CameraRotationSpeed = 15.0f;
	float m_CameraMovementSpeed = 3.0f;
	
	float m_CameraRotation = 0;
	glm::vec3 m_CameraPosition;

	Crane::Timestep m_Timestep;
	std::shared_ptr<Crane::Shader> m_Shader;
	std::shared_ptr<Crane::VertexArray> m_VertexArray;
	std::shared_ptr<Crane::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Crane::IndexBuffer> m_IndexBuffer;


	Crane::OrthographicCamera m_Camera;
};

class Sandbox : public Crane::Application
{
public:
	Sandbox()
	{
		PushLayer(new TriangleTest());
	}
	~Sandbox()
	{

	}

};


Crane::Application* Crane::CreateApplication() 
{
	return new Sandbox();
}