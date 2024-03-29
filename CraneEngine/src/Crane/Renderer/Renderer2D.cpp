#include "crpch.h"
#include "Renderer2D.h"

#include "Renderer.h"

namespace Crane
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->VertexArray = VertexArray::Create();
		
		float quadVertices[] = {
			// quad	
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		uint32_t quadIndices[] = {
			0,1,2,
			0,2,3
		};

		// create buffers + varray
		s_Data->VertexArray = Crane::VertexArray::Create();
		Crane::Ref<Crane::VertexBuffer> quadVertexBuffer = Crane::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		Crane::Ref<Crane::IndexBuffer> quadIndexBuffer = Crane::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));

		// setting the layout of the vertex buffer
		Crane::BufferLayout layout = {
			{ Crane::ShaderDataType::Float3 , "a_Position" }
		};

		quadVertexBuffer->SetLayout(layout);

		// add both buffers into Vertex Array
		s_Data->VertexArray->AddVertexBuffer(quadVertexBuffer);
		s_Data->VertexArray->SetIndexBuffer(quadIndexBuffer);

		//shaders
		s_Data->Shader = Crane::Shader::Create("assets/shaders/FlatColorShader.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetUniformMat4("u_ViewProjection", camera.GetViewProjMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, { color, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color)
	{
		DrawQuad(position, size, { color, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetUniformFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.1f));

		s_Data->Shader->SetUniformMat4("u_Model", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}