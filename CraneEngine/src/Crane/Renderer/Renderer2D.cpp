#include "crpch.h"

#include "Renderer2D.h"
#include "Renderer.h"

namespace Crane
{
	struct VertexData
	{
		VertexData(glm::vec3 position, glm::vec4 color, glm::vec2 textureCoordinates, float textureIndex)
			: Position(position), Color(color), TextureCoordinates(textureCoordinates), TextureIndex(textureIndex)
		{}
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoordinates;
		float TextureIndex;
	};

	struct Renderer2DStorage
	{
		uint32_t MaxQuads = 10'000;
		uint32_t MaxVertices = MaxQuads * 4;
		uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Check Renderer Capabilities 

		Ref<VertexArray> VertexArray; 
		Ref<VertexBuffer> VertexBuffer;

		Ref<Texture2D> WhiteTexture;
		Ref<Shader> ColorTextureShader;

		std::vector<VertexData> Vertices;

		uint32_t QuadsCount = 0;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // bcz 0 is white texture
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init()
	{
		CR_PROFILE_FUNCTION();
		// Create space for vertices that will be stored
		s_Data.Vertices.reserve(10);

		// create buffers + varray
		s_Data.VertexArray = Crane::VertexArray::Create();
		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(VertexData));

		// setting the layout of the vertex buffer
		BufferLayout layout = {
			{ Crane::ShaderDataType::Float3 , "a_Position" },
			{ Crane::ShaderDataType::Float4 , "a_Color" },
			{ Crane::ShaderDataType::Float2 , "a_TextureCoordinates" },
			{ Crane::ShaderDataType::Float , "a_TextureIndex" }
		};
		s_Data.VertexBuffer->SetLayout(layout);

		// add both buffers into Vertex Array
		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);


		// setting up all indices up to max even if they are not used
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
		// create the index buffer and set the data to be quad Indices
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.VertexArray->SetIndexBuffer(quadIndexBuffer);

		// delete the Index data
		delete[] quadIndices;

		// Create a white texture to use for colored quads
		s_Data.WhiteTexture = Texture2D::Create(1,1,4);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		
		// Create the shader
		s_Data.ColorTextureShader = Crane::Shader::Create("assets/shaders/ColorTextureShader.glsl");
		
		s_Data.ColorTextureShader->Bind();
		// lets add white texture in the TextureSlots Array
		s_Data.TextureSlots.at(0) = s_Data.WhiteTexture;

		// lets set all samplers in shader to be 0-32		
		int samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.ColorTextureShader->SetIntArray("u_Textures", s_Data.MaxTextureSlots, samplers);
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


		// clear data frame to frame or it will accumulate 
		s_Data.Vertices.clear();
		s_Data.QuadsCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		CR_PROFILE_FUNCTION();

		// lets bind all our textures we got from DrawQuads
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots.at(i)->Bind(i);

		// we set the data to be the vertices we added this frame
		s_Data.VertexBuffer->SetData(&s_Data.Vertices.at(0).Position.x, (uint32_t)s_Data.Vertices.size() * sizeof(VertexData));
		Flush();
	}

	void Renderer2D::Flush()
	{
		// finally draw
		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.QuadsCount * 6);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();
		
		// as we alredy bound the white texture in init we dont need to do that here
		//...
		// but we need to set textureIndex whitch will be zero since we want to have the color multiplied with the white texture
		float textureIndex = 0.0f;

		glm::vec4 finalColor(color, alpha);
		
		glm::vec3 br(position.x + size.x, position.y, 0.0f);
		glm::vec3 tr(position.x + size.x, position.y + size.y, 0.0f);
		glm::vec3 tl(position.x, position.y + size.y, 0.0f);
							
																			// texture slot
		s_Data.Vertices.emplace_back(position,finalColor, glm::vec2(0.0f, 0.0f), textureIndex);
		s_Data.Vertices.emplace_back(br, finalColor,      glm::vec2(1.0f, 0.0f), textureIndex);
		s_Data.Vertices.emplace_back(tr, finalColor,      glm::vec2(1.0f, 1.0f), textureIndex);
		s_Data.Vertices.emplace_back(tl, finalColor,      glm::vec2(0.0f, 1.0f), textureIndex);
	
	
		s_Data.QuadsCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();

		// logic to check if a texture was alredy occuping a textureSlot
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots.at(i)->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// if its new we add it to the array & add 1 to textureSlotIndex
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots.at(s_Data.TextureSlotIndex) = texture;
			s_Data.TextureSlotIndex++;
		}


		glm::vec4 finalColor(color, alpha);

		glm::vec3 br(position.x + size.x, position.y, 0.0f);
		glm::vec3 tr(position.x + size.x, position.y + size.y, 0.0f);
		glm::vec3 tl(position.x, position.y + size.y, 0.0f);

		s_Data.Vertices.emplace_back(position, finalColor, glm::vec2(0.0f, 0.0f), textureIndex);
		s_Data.Vertices.emplace_back(br, finalColor, glm::vec2(1.0f, 0.0f),       textureIndex);
		s_Data.Vertices.emplace_back(tr, finalColor, glm::vec2(1.0f, 1.0f),       textureIndex);
		s_Data.Vertices.emplace_back(tl, finalColor, glm::vec2(0.0f, 1.0f),       textureIndex);


		s_Data.QuadsCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();

		// as we alredy bound the white texture in init we dont need to do that here
		//...
		// but we need to set textureIndex whitch will be zero since we want to have the color multiplied with the white texture
		float textureIndex = 0.0f;

		glm::vec4 finalColor(color, alpha);

		glm::vec4 bl(position, 1.0f);
		glm::vec4 br(position.x + size.x, position.y, 0.0f, 1.0f);
		glm::vec4 tr(position.x + size.x, position.y + size.y, 0.0f, 1.0f);
		glm::vec4 tl(position.x, position.y + size.y, 0.0f, 1.0f);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f,0.0f,1.0f));

		bl = rotationMatrix * bl;
		br = rotationMatrix * br;
		tr = rotationMatrix * tr;
		tl = rotationMatrix * tl;

		s_Data.Vertices.emplace_back(glm::vec3(bl.x, bl.y, bl.z), finalColor, glm::vec2(0.0f, 0.0f), textureIndex);
		s_Data.Vertices.emplace_back(glm::vec3(br.x, br.y, br.z), finalColor, glm::vec2(1.0f, 0.0f), textureIndex);
		s_Data.Vertices.emplace_back(glm::vec3(tr.x, tr.y, tr.z), finalColor, glm::vec2(1.0f, 1.0f), textureIndex);
		s_Data.Vertices.emplace_back(glm::vec3(tl.x, tl.y, tl.z), finalColor, glm::vec2(0.0f, 1.0f), textureIndex);


		s_Data.QuadsCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec3& color, float alpha)
	{
		CR_PROFILE_FUNCTION();

		// logic to check if a texture was alredy occuping a textureSlot
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots.at(i)->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// if its new we add it to the array & add 1 to textureSlotIndex
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots.at(s_Data.TextureSlotIndex) = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::vec4 finalColor(color, alpha);

		glm::vec4 bl(position, 1.0f);
		glm::vec4 br(position.x + size.x, position.y, 0.0f, 1.0f);
		glm::vec4 tr(position.x + size.x, position.y + size.y, 0.0f, 1.0f);
		glm::vec4 tl(position.x, position.y + size.y, 0.0f, 1.0f);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		bl = rotationMatrix * bl;
		br = rotationMatrix * br;
		tr = rotationMatrix * tr;
		tl = rotationMatrix * tl;

		s_Data.Vertices.emplace_back(glm::vec3(bl.x, bl.y, bl.z), finalColor, glm::vec2(0.0f, 0.0f), textureIndex);
		s_Data.Vertices.emplace_back(glm::vec3(br.x, br.y, br.z), finalColor, glm::vec2(1.0f, 0.0f), textureIndex);
		s_Data.Vertices.emplace_back(glm::vec3(tr.x, tr.y, tr.z), finalColor, glm::vec2(1.0f, 1.0f), textureIndex);
		s_Data.Vertices.emplace_back(glm::vec3(tl.x, tl.y, tl.z), finalColor, glm::vec2(0.0f, 1.0f), textureIndex);


		s_Data.QuadsCount++;
	}

}