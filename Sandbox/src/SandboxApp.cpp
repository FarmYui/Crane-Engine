#include <Crane.h>
#include "imgui/imgui.h"

// temporary /////////////////////////////
#include "Platforms/OpenGL/OpenGLShader.h"
//////////////////////////////////////////

class TriangleTest : public Crane::Layer
{
public:
	TriangleTest()
		: Layer("Triangle Test"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		float vertices[] = {
			// triangle
			-0.6f, -0.5f, 0.0f,
			 0.6f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		uint32_t indices[] = {
			0,1,2,
		};

		// create buffers + varray
		m_VertexArray.reset(Crane::VertexArray::Create());
		m_VertexBuffer.reset(Crane::VertexBuffer::Create(vertices, sizeof(vertices)));
		m_IndexBuffer.reset(Crane::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// setting the layout of the vertex buffer
		Crane::BufferLayout layout = {
			{ Crane::ShaderDataType::Float3 , "a_Position" }
		};
		m_VertexBuffer->SetLayout(layout);

		// add both buffers into Vertex Array
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		float quadVertices[] = {
			// quad
		   -2.0f, -0.04f, 0.0f,
			2.0f, -0.04f, 0.0f,
		   	2.0f,  0.04f, 0.0f,
	 	   -2.0f,  0.04f, 0.0f
		};

		uint32_t quadIndices[] = {
			0,1,2,
			0,2,3
		};

		m_QuadVA.reset(Crane::VertexArray::Create());

		Crane::Ref<Crane::VertexBuffer> vb;
		vb.reset(Crane::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
		Crane::Ref<Crane::IndexBuffer> ib;
		ib.reset(Crane::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));

		vb->SetLayout(layout);

		// add both buffers into Vertex Array
		m_QuadVA->AddVertexBuffer(vb);
		m_QuadVA->SetIndexBuffer(ib);

		// shaders
		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_Model;
			uniform mat4 u_ViewProjection;			

			void main()
			{
				gl_Position = u_ViewProjection * u_Model * vec4(a_Position ,1.0f);
			})";

		std::string fragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color,1.0f);
			})";

		m_Shader.reset(Crane::Shader::Create(vertexSource, fragmentSource));
	}

	void OnUpdate(Crane::Timestep ts) override
	{
		m_Timestep = ts;

		if (Crane::Input::IsKeyPressed(CR_KEY_W))
		{
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts;
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts;
		}
		if (Crane::Input::IsKeyPressed(CR_KEY_S))
		{
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts;
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation + 90)) * m_CameraMovementSpeed * ts;
		}
		if (Crane::Input::IsKeyPressed(CR_KEY_A))
		{
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts;
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts;
		}
		if (Crane::Input::IsKeyPressed(CR_KEY_D))
		{
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts;
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * ts;
		}

		if (Crane::Input::IsMouseButtonPressed(CR_MOUSE_BUTTON_3))
		{
			float currentMouseX = Crane::Input::GetMouseX();
			m_CameraRotation += (m_PrevMouseX) ? ((currentMouseX - m_PrevMouseX) * m_CameraRotationSpeed) : 0;
			m_PrevMouseX = currentMouseX;
		}
		else
		{
			m_PrevMouseX = 0;
		}
		

		m_QuadTransform = glm::translate(glm::mat4(1.0f), m_QuadPosition);
		m_TriangleTransform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

	
		Crane::RendererCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Crane::RendererCommand::Clear();
		
		Crane::Renderer::BeginScene(m_Camera);

		m_Shader->Bind();
		std::dynamic_pointer_cast<Crane::OpenGLShader>(m_Shader)->SetUniformFloat3("u_Color", m_QuadColor);
		Crane::Renderer::Submit(m_QuadVA, m_Shader, m_QuadTransform);
		std::dynamic_pointer_cast<Crane::OpenGLShader>(m_Shader)->SetUniformFloat3("u_Color", m_TriangleColor);
		Crane::Renderer::Submit(m_VertexArray, m_Shader, m_TriangleTransform);

		Crane::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Info");

		ImGui::Text("FPS: %.1f", 1.0f/ m_Timestep);
		ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
		
		ImGui::End();



		ImGui::Begin("Attribs");

		ImGui::DragFloat3("Quad Position", &m_QuadPosition[0], 0.01f);
		ImGui::ColorEdit3("Quad Color", &m_QuadColor[0]);
		ImGui::Separator();
		ImGui::DragFloat3("Triangle Position", &m_TrianglePosition[0], 0.01f);
		ImGui::ColorEdit3("Triangle Color", &m_TriangleColor[0]);

		ImGui::End();
	}

	void OnEvent(Crane::Event& e) override
	{
	}

private:
	glm::vec3 m_TriangleColor = { 0.9f,0.2f,0.5f };
	glm::vec3 m_QuadColor = { 0.4f,0.7f,0.7f };

	glm::vec3 m_TrianglePosition = { 0,0,0 };
	glm::vec3 m_QuadPosition = { 0,0,0 };

	glm::mat4 m_TriangleTransform;
	glm::mat4 m_QuadTransform;

	float m_PrevMouseX = 0;

	float m_CameraRotationSpeed = 0.2f;
	float m_CameraMovementSpeed = 3.0f;
	
	float m_CameraRotation = 0;
	glm::vec3 m_CameraPosition;

	Crane::Timestep m_Timestep;
	Crane::Ref<Crane::Shader> m_Shader;
	Crane::Ref<Crane::VertexArray> m_VertexArray;
	Crane::Ref<Crane::VertexBuffer> m_VertexBuffer;
	Crane::Ref<Crane::IndexBuffer> m_IndexBuffer;

	Crane::Ref<Crane::VertexArray>m_QuadVA;


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