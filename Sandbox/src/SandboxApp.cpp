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
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.5f, 1.0f
		};

		uint32_t indices[] = {
			0,1,2,
		};

		// create buffers + varray
		m_TriangleVA = Crane::VertexArray::Create();
		m_VertexBuffer = Crane::VertexBuffer::Create(vertices, sizeof(vertices));
		m_IndexBuffer = Crane::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		// setting the layout of the vertex buffer
		Crane::BufferLayout layout = {
			{ Crane::ShaderDataType::Float3 , "a_Position" },
			{ Crane::ShaderDataType::Float2 , "a_TextureCoordinate"}
		};

		m_VertexBuffer->SetLayout(layout);

		// add both buffers into Vertex Array
		m_TriangleVA->AddVertexBuffer(m_VertexBuffer);
		m_TriangleVA->SetIndexBuffer(m_IndexBuffer);

		float quadVertices[] = {
			// quad
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		   	0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	 	   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		uint32_t quadIndices[] = {
			0,1,2,
			0,2,3
		};

		m_QuadVA = Crane::VertexArray::Create();

		Crane::Ref<Crane::VertexBuffer> vb = Crane::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		Crane::Ref<Crane::IndexBuffer> ib = Crane::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));

		vb->SetLayout(layout);

		// add both buffers into Vertex Array
		m_QuadVA->AddVertexBuffer(vb);
		m_QuadVA->SetIndexBuffer(ib);

		//shaders
		m_TextureShader = Crane::Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = Crane::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogo = Crane::Texture2D::Create("assets/textures/ChernoLogo.png");

		m_TextureShader->Bind();
		std::dynamic_pointer_cast<Crane::OpenGLShader>(m_TextureShader)->SetUniformInt("u_Texture", 0);
		

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

	
		Crane::RenderCommand::SetClearColor(m_ClearColor);
		Crane::RenderCommand::Clear();
		
		Crane::Renderer::BeginScene(m_Camera);

		m_TextureShader->Bind();
		
		m_Texture->Bind();
		std::dynamic_pointer_cast<Crane::OpenGLShader>(m_TextureShader)->SetUniformFloat3("u_Color", m_TriangleColor);
		Crane::Renderer::Submit(m_TriangleVA, m_TextureShader, m_TriangleTransform);
		
		std::dynamic_pointer_cast<Crane::OpenGLShader>(m_TextureShader)->SetUniformFloat3("u_Color", m_QuadColor);
		Crane::Renderer::Submit(m_QuadVA, m_TextureShader, m_QuadTransform);


		m_ChernoLogo->Bind();
		Crane::Renderer::Submit(m_QuadVA, m_TextureShader, m_QuadTransform);

		Crane::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Info");
		ImGui::Text("FPS: %.1f", 1.0f/ m_Timestep);
		ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
		ImGui::End();

		ImGui::Begin("Camera");
		ImGui::DragFloat3("Camera Position", &m_CameraPosition[0], 0.01f);
		ImGui::DragFloat("Camera Rotation", &m_CameraRotation, 1.0f);
		ImGui::End();
		
		ImGui::Begin("Scene");
		ImGui::DragFloat3("Quad Position", &m_QuadPosition[0], 0.01f);
		ImGui::ColorEdit3("Quad Color", &m_QuadColor[0]);
		ImGui::Separator();
		ImGui::DragFloat3("Triangle Position", &m_TrianglePosition[0], 0.01f);
		ImGui::ColorEdit3("Triangle Color", &m_TriangleColor[0]);
		ImGui::End();

		ImGui::Begin("Ambient");
		ImGui::ColorEdit3("Clear Color", &m_ClearColor[0]);
		ImGui::End();
	}

	void OnEvent(Crane::Event& e) override
	{
	}

private:
	glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec3 m_TriangleColor = { 0.9f,0.2f,0.5f };
	glm::vec3 m_QuadColor = { 1.0f,1.0f,1.0f };

	glm::vec3 m_TrianglePosition = { 0,1.0f,0 };
	glm::vec3 m_QuadPosition = { 0,0,0 };

	glm::mat4 m_TriangleTransform;
	glm::mat4 m_QuadTransform;

	float m_PrevMouseX = 0;

	float m_CameraRotationSpeed = 0.2f;
	float m_CameraMovementSpeed = 3.0f;
	
	float m_CameraRotation = 0;
	glm::vec3 m_CameraPosition;

	Crane::Timestep m_Timestep;

	Crane::Ref<Crane::Texture2D> m_Texture;
	Crane::Ref<Crane::Texture2D> m_ChernoLogo;

	Crane::Ref<Crane::Shader> m_TextureShader;
	Crane::Ref<Crane::VertexArray> m_TriangleVA;
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