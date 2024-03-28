#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(Crane::OrthographicCameraController(1.78f, true)) 
{};

void Sandbox2D::OnAttach()
{
	m_CameraController = Crane::OrthographicCameraController(1.78f, true);

	float vertices[] = {
		// triangle			//t coord
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.5f, 1.0f
	};

	uint32_t indices[] = {
		0,1,2,
	};

	// create buffers + varray
	m_TriangleVA = Crane::VertexArray::Create();
	Crane::Ref<Crane::VertexBuffer> vertexBuffer = Crane::VertexBuffer::Create(vertices, sizeof(vertices));
	Crane::Ref<Crane::IndexBuffer> indexBuffer = Crane::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	// setting the layout of the vertex buffer
	Crane::BufferLayout layout = {
		{ Crane::ShaderDataType::Float3 , "a_Position" },
		{ Crane::ShaderDataType::Float2 , "a_TextureCoordinate"}
	};

	vertexBuffer->SetLayout(layout);

	// add both buffers into Vertex Array
	m_TriangleVA->AddVertexBuffer(vertexBuffer);
	m_TriangleVA->SetIndexBuffer(indexBuffer);

	//shaders
	m_Shader = Crane::Shader::Create("assets/shaders/Texture.glsl");
	//texture
	m_Texture = Crane::Texture2D::Create("assets/textures/Checkerboard.png");;

}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnEvent(Crane::Event& e)
{
	m_CameraController.OnEvent(e);
}

void Sandbox2D::OnUpdate(Crane::Timestep ts)
{
	m_Timestep = ts;

	m_CameraController.OnUpdate(ts);

	Crane::RenderCommand::SetClearColor(m_ClearColor);
	Crane::RenderCommand::Clear();
	
	Crane::Renderer::BeginScene(m_CameraController.GetCamera());

	m_Shader->Bind();
	m_Texture->Bind();
	std::dynamic_pointer_cast<Crane::OpenGLShader>(m_Shader)->SetUniformFloat3("u_Color", m_TriangleColor);
	Crane::Renderer::Submit(m_TriangleVA, m_Shader, glm::mat4(1.0f));

	Crane::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Graphics Context");
	ImGui::Text("%s", Crane::Application::Get().GetWindow().GetContext()->GetInfo().c_str());
	ImGui::End();


	ImGui::Begin("Info");
	ImGui::Text("FPS: %.1f", 1.0f / m_Timestep);
	ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
	ImGui::End();

	ImGui::Begin("Ambient");
	ImGui::ColorEdit3("Clear Color", &m_ClearColor[0]);
	ImGui::ColorEdit3("Triangle Color", &m_TriangleColor[0]);
	ImGui::End();

}
