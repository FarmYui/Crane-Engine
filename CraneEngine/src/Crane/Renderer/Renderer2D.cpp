#include "crpch.h"
#include "Renderer2D.h"

#include "Renderer.h"

namespace Crane
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> VertexArray; 
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> ColorTextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->VertexArray = VertexArray::Create();
		
		float quadVertices[] = {
			// quad				//t coords
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
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
			{ Crane::ShaderDataType::Float3 , "a_Position" },
			{ Crane::ShaderDataType::Float2 , "a_TextureCoordinate" }
		};

		quadVertexBuffer->SetLayout(layout);

		// add both buffers into Vertex Array
		s_Data->VertexArray->AddVertexBuffer(quadVertexBuffer);
		s_Data->VertexArray->SetIndexBuffer(quadIndexBuffer);

		s_Data->WhiteTexture = Texture2D::Create(1,1,4);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->ColorTextureShader = Crane::Shader::Create("assets/shaders/ColorTextureShader.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->ColorTextureShader->Bind();
		s_Data->ColorTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	// pos vec2

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, { color, 1.0f }, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, rotation);
	}

	// pos vec3

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color, float rotation)
	{
		DrawQuad(position, size, { color, 1.0f }, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		s_Data->ColorTextureShader->SetFloat4("u_Color", color);

		//bind white texture here
		s_Data->WhiteTexture->Bind();
		//s_Data->ColorTextureShader->SetInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data->ColorTextureShader->SetMat4("u_Model", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation)
	{
		//set color to white
		s_Data->ColorTextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

		texture->Bind();
		//s_Data->ColorTextureShader->SetInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data->ColorTextureShader->SetMat4("u_Model", transform);

		texture->Bind(1);
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}


}