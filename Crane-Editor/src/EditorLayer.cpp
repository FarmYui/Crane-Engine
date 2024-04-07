#include "EditorLayer.h"
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

namespace Crane
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(OrthographicCameraController(1.78f, true))
	{};

	void EditorLayer::OnAttach()
	{
		CR_PROFILE_FUNCTION();

		FramebufferSpecification specification;
		specification.Width = 1280;
		specification.Height = 720;
		m_Framebuffer = Framebuffer::Create(specification);

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_TextureAtlas = Texture2D::Create("assets/game/textures/RPG_sheet.png");

		m_MapWidth = s_MapWidth;
		m_MapHeight = s_MapHeight;

		m_MapTiles['G'] = TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 1, 11 }, { 128,128 });
		m_MapTiles['D'] = TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 6, 11 }, { 128,128 });
		m_MapTiles['W'] = TextureRegion2D::CreateFromCoords(m_TextureAtlas, { 11, 11 }, { 128,128 });

		m_CameraController.SetZoomLevel(5.0f);
	}

	void EditorLayer::OnDetach()
	{
		CR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		CR_PROFILE_FUNCTION();
		m_Timestep = ts;

		m_CameraController.OnUpdate(ts);

		// Reset Stats
		Renderer2D::ResetStats();

		// Clear
		{
			CR_PROFILE_SCOPE("Renderer Clear");

			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(m_ClearColor);
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

	void EditorLayer::OnImGuiRender()
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
		ImVec2 ImGuiViewportSize = ImGui::GetContentRegionAvail();
		glm::vec2 viewportSize = { ImGuiViewportSize.x, ImGuiViewportSize.y };

		if (m_ViewportSize != viewportSize)
		{
			m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_ViewportSize = viewportSize;
			m_CameraController.Resize(viewportSize.x, viewportSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImGuiViewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
		
	}
}