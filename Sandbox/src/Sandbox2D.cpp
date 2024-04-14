#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Crane
{
	class Tile
	{
	public:
		Tile(char type, Ref<Texture2D>textureAtlas, glm::vec2 coords, glm::vec2 spriteOffset)
			: m_Type(type)
		{
			m_Region = TextureRegion2D::CreateFromCoords(textureAtlas, coords, spriteOffset);
		}

		char GetType() const { return m_Type; }
		const Ref<TextureRegion2D>& GetRegion() const { return m_Region; }

	private:
		char m_Type;
		Ref<TextureRegion2D> m_Region;
	};


	class TileTransition
	{
	public:
		TileTransition(char outertype, char innerType, char* map, uint32_t mapWidth, uint32_t mapHeight)
			: m_OuterType(outertype), m_InnerType(innerType), m_MapWidth(mapWidth), m_MapHeight(mapHeight)
		{
			m_Map = map;
		}

		const std::vector<Tile>& GetTransitionTiles(Ref<Texture2D>textureAtlas, glm::vec2 trCoords, glm::vec2 spriteOffset)
		{
			uint32_t spriteX = 3;
			uint32_t spriteY = 3;

			for (uint32_t y = 0; y < spriteY; y++)
			{
				for (uint32_t x = 0; x < spriteX; x++)
				{
					m_TransitionTiles.emplace_back(Tile(m_NextChar, textureAtlas, { trCoords.x + x, trCoords.y - y }, spriteOffset));
					m_NextChar = (int)m_NextChar + 1;
					if (m_NextChar == m_InnerType || m_NextChar == m_OuterType)
						m_NextChar = (int)m_NextChar + 1;
				}
			}
			return m_TransitionTiles;
		}

		const std::vector<Tile>& GetSmallTransitionTiles(Ref<Texture2D>textureAtlas, glm::vec2 trCoords, glm::vec2 spriteOffset)
		{
			uint32_t spriteX = 2;
			uint32_t spriteY = 2;

			for (uint32_t y = 0; y < spriteY; y++)
			{
				for (uint32_t x = 0; x < spriteX; x++)
				{
					m_SmallTransitionTiles.emplace_back(Tile(m_NextChar, textureAtlas, { trCoords.x + x, trCoords.y - y }, spriteOffset));
					m_NextChar = (int)m_NextChar + 1;
					if (m_NextChar == m_InnerType || m_NextChar == m_OuterType)
						m_NextChar = (int)m_NextChar + 1;
				}
			}
			return m_SmallTransitionTiles;
		}


		void UpdateMap()
		{
			for (uint32_t y = 1; y < m_MapHeight - 1; y++)
			{
				for (uint32_t x = 1; x < m_MapWidth - 1; x++)
				{
					char topleft = m_Map[x - 1 + (y - 1) * m_MapWidth];
					char top = m_Map[x + (y - 1) * m_MapWidth];
					char topright = m_Map[x + 1 + (y - 1) * m_MapWidth];

					char left = m_Map[x - 1 + y * m_MapWidth];
					char current = m_Map[x + y * m_MapWidth];
					char right = m_Map[x + 1 + y * m_MapWidth];

					char bottomleft = m_Map[x - 1 + (y + 1) * m_MapWidth];
					char bottom = m_Map[x + (y + 1) * m_MapWidth];
					char bottomright = m_Map[x + 1 + (y + 1) * m_MapWidth];


					if (current != m_InnerType)
						continue;

					if (topleft == current && top == current && topright == current && left == current && right == current && bottomleft == current && bottom == current && bottomright == current)
						continue;

					if (topleft != current && top != current && topright != current && left != current && right != current && bottomleft != current && bottom != current && bottomright != current)
						continue;


					uint32_t indexX = MAXUINT32;
					uint32_t indexY = MAXUINT32;

					uint32_t smallindexX = MAXUINT32;
					uint32_t smallindexY = MAXUINT32;

					// top checks

					if (topleft != current && top != current && left != current)
					{
						indexY = 0;
						indexX = 0;
					}

					else if (topright != current && top != current && right != current)
					{
						indexY = 0;
						indexX = 2;
					}

					// question: do we also have to check whats current?
					else if (top != current)
					{
						indexY = 0;
						indexX = 1;
					}

					//bottom checks

					else if (left != current && bottomleft != current && bottom != current)
					{
						indexY = 2;
						indexX = 0;
					}

					else if (right != current && bottomright != current && bottom != current)
					{
						indexY = 2;
						indexX = 2;
					}

					// question: do we also have to check whats current?
					else if (bottom != current)
					{
						indexY = 2;
						indexX = 1;
					}

					// sides checks

					else if (left != current)
					{
						indexY = 1;
						indexX = 0;
					}


					else if (right != current)
					{
						indexY = 1;
						indexX = 2;
					}

					//small tiles check

					else if (right == current && bottom == current && bottomright != current)
					{
						smallindexX = 0;
						smallindexY = 0;
					}

					else if (left == current && bottom == current && bottomleft != current)
					{
						smallindexX = 1;
						smallindexY = 0;
					}

					else if (right == current && top == current && topright != current)
					{
						smallindexX = 0;
						smallindexY = 1;
					}

					else if (left == current && top == current && topleft != current)
					{
						smallindexX = 1;
						smallindexY = 1;
					}


					if (indexX != MAXUINT32 && indexY != MAXUINT32)
					{
						char final = m_TransitionTiles.at(indexY * 3 + indexX).GetType();
						m_TileData.push_back({ indexX, indexY, x,y, final });
					}

					if (smallindexX != MAXUINT32 && smallindexY != MAXUINT32)
					{
						char smallfinal = m_SmallTransitionTiles.at(smallindexY * 2 + smallindexX).GetType();
						m_TileData.push_back({ smallindexX, smallindexY, x,y, smallfinal });
					}
				}
			}


			for (auto& td : m_TileData)
				m_Map[td.iterX + td.iterY * m_MapWidth] = td.final;

		}

		struct TileData
		{
			uint32_t indexX;
			uint32_t indexY;

			uint32_t iterX;
			uint32_t iterY;

			char final;
		};


	private:
		char* m_Map;
		uint32_t m_MapWidth;
		uint32_t m_MapHeight;

		std::vector<Tile> m_TransitionTiles;
		std::vector<Tile> m_SmallTransitionTiles;

		char m_OuterType;
		char m_InnerType;

		static char m_NextChar;

		std::vector<TileData> m_TileData;
	};

}

char Crane::TileTransition::m_NextChar = 'A';


static const uint32_t s_MapWidth = 24;
static const uint32_t s_MapHeight = 14;
static char s_Map[] =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWGGGGGGGGGGWWWWWW"
"WWWWWWGGGGDDGGGGGGGWWWWW"
"WWWWWWGGGDDDGGGGGGGGWWWW"
"WWWWWGGGGDDDGGGGGGGGGWWW"
"WWWWWGGGGGGGGGGGGGGGGWWW"
"WWWWWGGGGGGGGGGGGGGGGWWW"
"WWWWWWGGGGGGGWWWGGGGWWWW"
"WWWWWWGGGGGGGWWWWGGGWWWW"
"WWWWWWWGGGGGGWWWWGGWWWWW"
"WWWWWWWWGGGGGGGGGGWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";


namespace Crane
{
	
	Sandbox2D::Sandbox2D()
		: Layer("Sandbox2D"), m_CameraController(OrthographicCameraController(1.78f, true))
	{};

	void Sandbox2D::OnAttach()
	{
		CR_PROFILE_FUNCTION();

		FramebufferSpecification specification;
		specification.Width = 1280;
		specification.Height = 720;
		m_Framebuffer = Framebuffer::Create(specification);

		m_TextureAtlas = Texture2D::Create("assets/game/textures/RPG_sheet.png");

		m_MapWidth = s_MapWidth;
		m_MapHeight = s_MapHeight;

		{
			TileTransition tt('G', 'D', s_Map, s_MapWidth, s_MapHeight);
			const std::vector<Tile>& Tiles = tt.GetTransitionTiles(m_TextureAtlas, { 5,12 }, { 128,128 });
			for (auto& tile : Tiles)
				m_MapTiles[tile.GetType()] = tile.GetRegion();

			const std::vector<Tile>& SmallTiles = tt.GetSmallTransitionTiles(m_TextureAtlas, { 8,12 }, { 128,128 });
			for (auto& tile : SmallTiles)
				m_MapTiles[tile.GetType()] = tile.GetRegion();

			tt.UpdateMap();
		}

		{
			TileTransition tt('G', 'W', s_Map, s_MapWidth, s_MapHeight);
			const std::vector<Tile>& Tiles = tt.GetTransitionTiles(m_TextureAtlas, { 10,12 }, { 128,128 });
			for (auto& tile : Tiles)
				m_MapTiles[tile.GetType()] = tile.GetRegion();

			const std::vector<Tile>& SmallTiles = tt.GetSmallTransitionTiles(m_TextureAtlas, { 13,12 }, { 128,128 });
			for (auto& tile : SmallTiles)
				m_MapTiles[tile.GetType()] = tile.GetRegion();

			tt.UpdateMap();
		}


		m_MapTiles['G'] = TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 1, 11 }, { 128,128 });
		m_MapTiles['D'] = TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 6, 11 }, { 128,128 });
		m_MapTiles['W'] = TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 11, 11 }, { 128,128 });

		m_CameraController.SetZoomLevel(5.0f);
	}

	void Sandbox2D::OnDetach()
	{
		CR_PROFILE_FUNCTION();
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void Sandbox2D::OnUpdate(Timestep ts)
	{
		CR_PROFILE_FUNCTION();
		m_Timestep = ts;

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
		}


		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Reset Stats
		Renderer2D::ResetStats();

		// Clear
		{
			CR_PROFILE_SCOPE("Renderer Clear");

			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(glm::vec4(0.2f));
			RenderCommand::Clear();
		}

		// Draw
		{
			CR_PROFILE_SCOPE("Renderer Draw");


			Renderer2D::BeginScene(m_CameraController.GetCamera());

			for (uint32_t y = 0; y < s_MapHeight; y++)
			{
				for (uint32_t x = 0; x < m_MapWidth; x++)
				{
					char tileType = s_Map[x + m_MapWidth * y];
					Ref<TextureRegion2D> region;
					if (m_MapTiles.find(tileType) != m_MapTiles.end())
						region = m_MapTiles.at(tileType);
					else
						region = TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 0, 0 }, { 128,128 });

					Renderer2D::DrawQuad(glm::vec3(x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, 0.0f), glm::vec2(1.0f), m_TextureAtlas, region);
				}
			}

			Renderer2D::EndScene();

			m_Framebuffer->Unbind();
		}
	}

	void Sandbox2D::OnImGuiRender()
	{
		CR_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->AllowEvents(m_ViewportFocused && m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}
}