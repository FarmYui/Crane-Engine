#include "crpch.h"

#include "Renderer2D.h"
#include "Renderer.h"

#include "Crane/Renderer/TextureRegion2D.h"

namespace Crane
{
	struct VertexData
	{
		VertexData(glm::vec3 position, glm::vec4 color, glm::vec2 textureCoordinates, float textureIndex, int entityID)
			: Position(position), Color(color), TextureCoordinates(textureCoordinates), TextureIndex(textureIndex), EntityID(entityID)
		{}
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoordinates;
		float TextureIndex;

		// Editor only
		int EntityID;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MaxQuads = 10'000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Check Renderer Capabilities 

		Ref<VertexArray> VertexArray; 
		Ref<VertexBuffer> VertexBuffer;
		Ref<Shader> ColorTextureShader;

		Ref<Texture2D> WhiteTexture;
		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // bcz 0 is white texture

		// TODO: make this an array
		std::vector<VertexData> Vertices;
		glm::vec4 QuadVertexPositions[4];

		uint32_t QuadsCount = 0;
		Renderer2D::Statistics Stats;
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init()
	{
		CR_PROFILE_FUNCTION();
		// Reserve space for vertices that will be stored
		s_Data.Vertices.reserve(s_Data.MaxVertices);

		// create buffers + varray
		s_Data.VertexArray = Crane::VertexArray::Create();
		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(VertexData));

		// setting the layout of the vertex buffer
		BufferLayout layout = {
			{ Crane::ShaderDataType::Float3 , "a_Position"		     },
			{ Crane::ShaderDataType::Float4 , "a_Color"			 	 },
			{ Crane::ShaderDataType::Float2 , "a_TextureCoordinates" },
			{ Crane::ShaderDataType::Float  , "a_TextureIndex"		 },
			{ Crane::ShaderDataType::Int    , "a_EntityID"			 }	
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


		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		CR_PROFILE_FUNCTION();

	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		CR_PROFILE_FUNCTION();
		
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.ColorTextureShader->Bind();
		s_Data.ColorTextureShader->SetMat4("u_ViewProjection", viewProj);

		// clears data
		StartNewBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		CR_PROFILE_FUNCTION();

		glm::mat4& viewProj = camera.GetViewProjection();

		s_Data.ColorTextureShader->Bind();
		s_Data.ColorTextureShader->SetMat4("u_ViewProjection", viewProj);

		// clears data
		StartNewBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CR_PROFILE_FUNCTION();
		s_Data.ColorTextureShader->Bind();
		s_Data.ColorTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjMatrix());

		// clears data
		StartNewBatch();
	}

	void Renderer2D::StartNewBatch()
	{
		// clear data frame to frame or it will accumulate 
		s_Data.Vertices.clear();
		s_Data.QuadsCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		CR_PROFILE_FUNCTION();

		if (s_Data.QuadsCount == 0)
			return; // Nothing to bind or to draw 

		// lets bind all our textures we got from DrawQuads
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots.at(i)->Bind(i);

		// we set the data to be the vertices we added this frame
		s_Data.VertexBuffer->SetData(&s_Data.Vertices.at(0).Position.x, (uint32_t)s_Data.Vertices.size() * sizeof(VertexData));
		Flush();


		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::Flush()
	{
		// finally draw
		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.QuadsCount * 6);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		CR_PROFILE_FUNCTION();

		//model * vtx pos
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));
		
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		CR_PROFILE_FUNCTION();

		//model * vtx pos
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		DrawQuad(transform, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const Ref<TextureRegion2D>& textureRegion, const glm::vec4& color)
	{
		CR_PROFILE_FUNCTION();

		//model * vtx pos
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		DrawQuad(transform, texture, textureRegion, color);
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		CR_PROFILE_FUNCTION();

		//model * vtx pos
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		CR_PROFILE_FUNCTION();

		//model * vtx pos
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		DrawQuad(transform, texture, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const Ref<TextureRegion2D>& textureRegion, const glm::vec4& color)
	{
		CR_PROFILE_FUNCTION();

		//model * vtx pos
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		DrawQuad(transform, texture, textureRegion, color);
	}


	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		CR_PROFILE_FUNCTION();

		glm::vec2 textureCoordinates[] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		// logic to stop adding vertices on a full vertex and index buffer
		if (s_Data.QuadsCount == s_Data.MaxQuads)
		{	// we end the scene and start a new one with the same camera data
			EndScene();
			StartNewBatch();
		}

		// as we alredy bound the white texture in init we dont need to do that here
		//...
		// but we need to set textureIndex whitch will be zero since we want to have the color multiplied with the white texture
		float textureIndex = 0.0f;

		for (uint32_t i = 0; i < 4; i++)
			s_Data.Vertices.emplace_back(transform * s_Data.QuadVertexPositions[i], color, textureCoordinates[i], textureIndex, entityID);


		s_Data.QuadsCount++;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, int entityID)
	{
		CR_PROFILE_FUNCTION();

		glm::vec2 textureCoordinates[] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		// logic to stop adding vertices on a full vertex and index buffer
		if (s_Data.QuadsCount == s_Data.MaxQuads)
		{	// we end the scene and start a new one with the same camera data
			EndScene();
			StartNewBatch();
		}

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
			if (s_Data.TextureSlotIndex == Renderer2DStorage::MaxTextureSlots) // if we dont have any more space for textures
			{// we end the scene and start a new one with the same camera data
				EndScene();
				StartNewBatch();
			}
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots.at(s_Data.TextureSlotIndex) = texture;
			s_Data.TextureSlotIndex++;
		}

		// Adding vertex data into array  
		for (uint32_t i = 0; i < 4; i++)
			s_Data.Vertices.emplace_back(transform * s_Data.QuadVertexPositions[i], color, textureCoordinates[i], textureIndex, entityID);


		s_Data.QuadsCount++;
		// this is for stats
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const Ref<TextureRegion2D>& textureRegion, const glm::vec4& color, int entityID)
	{
		CR_PROFILE_FUNCTION();

		const std::array<glm::vec2, 4>& textureCoordinates = textureRegion->GetTextureCoordinates();

		// logic to stop adding vertices on a full vertex and index buffer
		if (s_Data.QuadsCount == s_Data.MaxQuads)
		{	// we end the scene and start a new one with the same camera data
			EndScene();
			StartNewBatch();
		}

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
			if (s_Data.TextureSlotIndex == Renderer2DStorage::MaxTextureSlots) // if we dont have any more space for textures
			{// we end the scene and start a new one with the same camera data
				EndScene();
				StartNewBatch();
			}
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots.at(s_Data.TextureSlotIndex) = texture;
			s_Data.TextureSlotIndex++;
		}

		// Adding vertex data into array  
		for (uint32_t i = 0; i < 4; i++)
			s_Data.Vertices.emplace_back(transform * s_Data.QuadVertexPositions[i], color, textureCoordinates[i], textureIndex, entityID);


		s_Data.QuadsCount++;
		// this is for stats
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& spriteRendererComponent, int entityID)
	{
		DrawQuad(transform, spriteRendererComponent.Color, entityID);
	}



	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}


}