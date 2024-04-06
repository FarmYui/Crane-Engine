#include "Sandbox2D.h"
#include <imgui/imgui.h>

static const uint32_t s_MapWidth = 24;
static const uint32_t s_MapHeight = 12;
static const char* s_Map =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWGGGGGGGGGGWWWWWW"
"WWWWWWGGGGDDGGGGGGGWWWWW"
"WWWWWGGGGDDDGGGGGGGGWWWW"
"WWWWGGGGGDDDGGGGGGGGGWWW"
"WWWWGGGGGGGGGGGGGGGGGGWW"
"WWWWWGGGGGGGGGGGGGGGGGWW"
"WWWWWWGGGGGGGWWWGGGGGWWW"
"WWWWWWGGGGGGGWWWWGGGWWWW"
"WWWWWWWGGGGGWWWWWGGWWWWW"
"WWWWWWWWGGGGGGGGGGWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(Crane::OrthographicCameraController(1.78f, true)) 
{};

void Sandbox2D::OnAttach()
{
	CR_PROFILE_FUNCTION();

	m_TextureAtlas = Crane::Texture2D::Create("assets/game/textures/RPG_sheet.png");

	m_MapWidth = s_MapWidth;
	m_MapHeight = s_MapHeight;

	m_MapTiles['G'] = Crane::TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 1, 11 }, {128,128});
	m_MapTiles['D'] = Crane::TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 6, 11 }, {128,128});
	m_MapTiles['W'] = Crane::TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 11, 11 }, { 128,128 });

	m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{
	CR_PROFILE_FUNCTION();	
}

void Sandbox2D::OnEvent(Crane::Event& e)
{
	m_CameraController.OnEvent(e);
}

void Sandbox2D::OnUpdate(Crane::Timestep ts)
{
	CR_PROFILE_FUNCTION();
	m_Timestep = ts;

	m_CameraController.OnUpdate(ts);

	// Reset Stats
	Crane::Renderer2D::ResetStats();

	// Clear
	{
		CR_PROFILE_SCOPE("Renderer Clear");
		Crane::RenderCommand::SetClearColor(m_ClearColor);
		Crane::RenderCommand::Clear();
	}

	// Draw
	{
		CR_PROFILE_SCOPE("Renderer Draw");
#if TEST
		Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());
		/*
		Crane::Renderer2D::DrawRotatedQuad({ m_QuadPosition.x, m_QuadPosition.y, m_QuadPosition.z}, m_QuadSize, m_QuadRotation, m_Texture, m_QuadColor);
		Crane::Renderer2D::DrawRotatedQuad(glm::vec3(m_QuadPosition.x - 1.1f, 0.0f,0.0f), m_QuadSize, m_QuadRotation, m_Texture2);
		*/
		Crane::Renderer2D::DrawRotatedQuad(glm::vec3(m_QuadPosition.x, 0.0f, -0.1f), { m_QuadSize.x + 0.5f, m_QuadSize.y * 3.0f + 0.25f }, m_QuadRotation, m_QuadColor);

		for (uint32_t i = 1; i < 4; i++)
		{
			float increment = 0.2f * (float)i;
			Crane::Renderer2D::DrawRotatedQuad(glm::vec3(m_QuadPosition.x, increment + m_QuadPosition.y - 0.4f, increment),m_QuadSize, m_QuadRotation, m_Texture3, glm::vec3(m_QuadColor), increment);
		}
		
		Crane::Renderer2D::EndScene();

		Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				float r = (x + 5.0f) / 10.0f;
				float g = (y + 5.0f) / 10.0f;
				Crane::Renderer2D::DrawQuad(glm::vec3(x, y, -0.1f), glm::vec2(0.45f), glm::vec3(r, g, 0.3f));
			}
		}
		Crane::Renderer2D::EndScene();
#endif

		Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (uint32_t y = 0; y < s_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_Map[x + m_MapWidth * y];
				Crane::TextureRegion2D region;
				if (m_MapTiles.find(tileType) != m_MapTiles.end())
					region = m_MapTiles.at(tileType);
				else
					region = Crane::TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 0, 0 }, { 128,128 });
				
				Crane::Renderer2D::DrawQuad(glm::vec3(x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f,0.0f), glm::vec2(1.0f), m_TextureAtlas, region);
			}
		}
			

		Crane::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	CR_PROFILE_FUNCTION();

	auto& stats = Crane::Renderer2D::GetStats();

	ImGui::Begin("Info");
	ImGui::Text("FPS: %.1f", 1.0f / m_Timestep);
	ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
	ImGui::End();

	ImGui::Begin("Stats");
	ImGui::Text("Renderer2D Stats");
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("QuadCount: %d", stats.QuadCount);
	ImGui::Text("Vertices : %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices  : %d", stats.GetTotalIndexCount());
	ImGui::End();


	ImGui::Begin("Scene");
	ImGui::DragFloat3("Quad Position", &m_QuadPosition[0], 0.01f);
	ImGui::DragFloat("Quad Rotation", &m_QuadRotation);
	ImGui::DragFloat3("Quad Size", &m_QuadSize[0], 0.01f);
	ImGui::ColorEdit3("Quad Color", &m_QuadColor[0]);
	ImGui::End();

	ImGui::Begin("Ambient");
	ImGui::ColorEdit3("Clear Color", &m_ClearColor[0]);
	
	ImGui::End();

}
