#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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

	m_CheckerboardTexture = Crane::Texture2D::Create("assets/textures/Checkerboard.png");
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

	{
		ImGui::Begin("Settings");

		auto stats = Crane::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_QuadColor));
		ImGui::End();
	}

}
