#include "Sandbox2D.h"
#include "Crane/Core/EntryPoint.h"

#ifdef EXAMPLE_LAYER
class ExampleLayer : public Crane::Layer
{
public:
	ExampleLayer()
		: Layer("Triangle Test"), m_CameraController(1.78f,true)
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
		auto& textureShader = m_ShaderLib.Load("assets/shaders/Texture.glsl");

		m_Texture = Crane::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogo = Crane::Texture2D::Create("assets/textures/ChernoLogo.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
		

	}

	void OnEvent(Crane::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	void OnUpdate(Crane::Timestep ts) override
	{
		m_Timestep = ts;

		m_QuadTransform = glm::translate(glm::mat4(1.0f), m_QuadPosition);
		m_TriangleTransform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);

		m_CameraController.OnUpdate(ts);

		Crane::RenderCommand::SetClearColor(m_ClearColor);
		Crane::RenderCommand::Clear();
		
		Crane::Renderer::BeginScene(m_CameraController.GetCamera());

		auto& textureShader = m_ShaderLib.Get("Texture");
		textureShader->Bind();
		
		m_Texture->Bind();
		textureShader->SetFloat3("u_Color", m_TriangleColor);
		Crane::Renderer::Submit(m_TriangleVA, textureShader, m_TriangleTransform);
		
		textureShader->SetFloat3("u_Color", m_QuadColor);
		Crane::Renderer::Submit(m_QuadVA, textureShader, m_QuadTransform);


		m_ChernoLogo->Bind();
		Crane::Renderer::Submit(m_QuadVA, textureShader, m_QuadTransform);

		Crane::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Graphics Context");
		ImGui::Text("%s", Crane::Application::Get().GetWindow().GetContext()->GetInfo().c_str());
		ImGui::End();


		ImGui::Begin("Info");
		ImGui::Text("FPS: %.1f", 1.0f/ m_Timestep);
		ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
		ImGui::End();

		ImGui::Begin("Camera");
		//ImGui::DragFloat3("Camera Position", m_CameraController.GetCamera().GetPosition()[0], 0.01f);
		//ImGui::DragFloat("Camera Rotation", &m_CameraController.GetCamera().GetRotation(), 1.0f);
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

private:
	glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec3 m_TriangleColor = { 0.9f,0.2f,0.5f };
	glm::vec3 m_QuadColor = { 1.0f,1.0f,1.0f };

	glm::vec3 m_TrianglePosition = { 0,1.0f,0 };
	glm::vec3 m_QuadPosition = { 0,0,0 };

	glm::mat4 m_TriangleTransform;
	glm::mat4 m_QuadTransform;

	Crane::Timestep m_Timestep;

	Crane::Ref<Crane::Texture2D> m_Texture;
	Crane::Ref<Crane::Texture2D> m_ChernoLogo;

	Crane::ShaderLibrary m_ShaderLib;
	Crane::Ref<Crane::VertexArray> m_TriangleVA;
	Crane::Ref<Crane::VertexBuffer> m_VertexBuffer;
	Crane::Ref<Crane::IndexBuffer> m_IndexBuffer;

	Crane::Ref<Crane::VertexArray>m_QuadVA;

	Crane::OrthographicCameraController m_CameraController;
};
#endif

class Sandbox : public Crane::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{

	}

};


Crane::Application* Crane::CreateApplication() 
{
	return new Sandbox();
}