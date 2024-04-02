#include "crpch.h"

#include "Renderer2D.h"
#include "Renderer.h"

namespace Crane
{
	struct VertexData
	{
		VertexData(glm::vec3 position, glm::vec4 color)
			: Position(position), Color(color)
		{}
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Renderer2DStorage
	{
		uint32_t MaxQuads = 10'000;
		uint32_t MaxVertices = MaxQuads * 4;
		uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> VertexArray; 
		Ref<VertexBuffer> VertexBuffer;

		Ref<Texture2D> WhiteTexture;
		Ref<Shader> ColorTextureShader;

		std::vector<VertexData> Vertices;

		uint32_t QuadsCount = 0;
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init()
	{
		CR_PROFILE_FUNCTION();

		s_Data.Vertices.reserve(10);

		// create buffers + varray
		s_Data.VertexArray = Crane::VertexArray::Create();
		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(VertexData));

		// setting the layout of the vertex buffer
		BufferLayout layout = {
			{ Crane::ShaderDataType::Float3 , "a_Position" },
			{ Crane::ShaderDataType::Float4 , "a_Color" }
		};

		s_Data.VertexBuffer->SetLayout(layout);

		// add both buffers into Vertex Array
		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);


		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0; 
		for (uint32_t i = 0; i < s_Data.MaxIndices; i+=6)
		{
			quadIndices[i + 0] = offset;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset;
			quadIndices[i + 4] = offset + 2;
			quadIndices[i + 5] = offset + 3;
			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.VertexArray->SetIndexBuffer(quadIndexBuffer);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1,1,4);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.ColorTextureShader = Crane::Shader::Create("assets/shaders/ColorTextureShader.glsl");
	}

	void Renderer2D::Shutdown()
	{
		CR_PROFILE_FUNCTION();

	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CR_PROFILE_FUNCTION();
		s_Data.ColorTextureShader->Bind();
		s_Data.ColorTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjMatrix());
	}

	void Renderer2D::EndScene()
	{
		CR_PROFILE_FUNCTION();

		s_Data.VertexBuffer->SetData(&s_Data.Vertices.at(0).Position.x, (uint32_t)s_Data.Vertices.size() * sizeof(VertexData));
		Flush();
		s_Data.Vertices.clear();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.QuadsCount * 6);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();
		
		glm::vec4 finalColor(color, alpha);
		
		glm::vec3 br(position.x + size.x, position.y, 0.0f);
		glm::vec3 tr(position.x + size.x, position.y + size.y, 0.0f);
		glm::vec3 tl(position.x, position.y + size.y, 0.0f);

		s_Data.Vertices.emplace_back(position,finalColor);
		s_Data.Vertices.emplace_back(br, finalColor);
		s_Data.Vertices.emplace_back(tr, finalColor);
		s_Data.Vertices.emplace_back(tl, finalColor);
	
	
		s_Data.QuadsCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();
		//set color 
		s_Data.ColorTextureShader->SetFloat4("u_Color", { color, alpha });

		texture->Bind();
		//s_Data.ColorTextureShader->SetInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data.ColorTextureShader->SetMat4("u_Model", transform);

		texture->Bind(1);
		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();
		s_Data.ColorTextureShader->SetFloat4("u_Color", { color,alpha });

		//bind white texture here
		s_Data.WhiteTexture->Bind();
		//s_Data.ColorTextureShader->SetInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data.ColorTextureShader->SetMat4("u_Model", transform);

		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();
		//set color 
		s_Data.ColorTextureShader->SetFloat4("u_Color", glm::vec4(color.r, color.g, color.b, alpha));

		texture->Bind();
		//s_Data.ColorTextureShader->SetInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data.ColorTextureShader->SetMat4("u_Model", transform);

		texture->Bind();
		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);

	}

}