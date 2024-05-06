#include "EditorLayer.h"
#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include "Crane/Scene/SceneSerializer.h"
#include "Crane/Utils/PlatformUtils.h"
#include "Crane/Math/Math.h"

namespace Crane
{

	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{};

	void EditorLayer::OnAttach()
	{
		CR_PROFILE_FUNCTION();

		FramebufferSpecification specification;
		specification.Width = 1280;
		specification.Height = 720;
		specification.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		m_Framebuffer = Framebuffer::Create(specification);

		m_ActiveScene = CreateRef<Scene>();

		m_SceneHeirarchyPanel.SetScene(m_ActiveScene);

		m_PlayButtonTexture = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_StopButtonTexture = Texture2D::Create("Resources/Icons/StopButton.png");
	}

	void EditorLayer::OnDetach()
	{
		CR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(CR_BIND_EVENT_FN(EditorLayer::OnKeyPressed)); 
		dispatcher.Dispatch<MouseButtonPressedEvent>(CR_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));

	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool controlPressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shiftPressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		// Scene Shortcuts
		case Key::N:
			if (controlPressed)
				NewScene();
			break;
		
		case Key::O:
			if (controlPressed)
				OpenScene();
			break;

		case Key::S:
			if (controlPressed && shiftPressed)
				SaveSceneAs();
			break;
		
		// Gizmo Shortcuts
		case Key::Q:
			m_GizmoMode = -1;
			break;
		case Key::W:
			m_GizmoMode = (int32_t)ImGuizmo::TRANSLATE;
			break;
		case Key::E:
			m_GizmoMode = (int32_t)ImGuizmo::ROTATE;
			break;
		case Key::R:
			m_GizmoMode = (int32_t)ImGuizmo::SCALE;
			break;
		// Cicle trough gizmos
		case Key::Tab:
			m_GizmoMode++;
			if (m_GizmoMode == 3)
				m_GizmoMode = 0;
			break;

		default:
			return false;
		}

		// this is actually wrong bcz if we dont press control no code will run
		return true;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
			{
				m_SelectEntity = true;

				return true;
			}
		}
		return false;
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		CR_PROFILE_FUNCTION();
		m_Timestep = ts;

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewportFocused && !ImGuizmo::IsUsing())
			m_EditorCamera.OnUpdate(ts);

		// Reset Stats
		Renderer2D::ResetStats();

		// Clear
		{
			CR_PROFILE_SCOPE("Renderer Clear");

			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(m_ClearColor);
			RenderCommand::Clear();

			// Clear entity id attachment 
			m_Framebuffer->ClearAttachment(1, -1);
		}

		// Draw
		{
			CR_PROFILE_SCOPE("Renderer Draw");

			if (m_SceneState == SceneState::Edit)
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			if (m_SceneState == SceneState::Play)
				m_ActiveScene->OnUpdateRuntime(ts);

			// this has to be called only if the framebuffer is bound
			if (m_SelectEntity)
				SelectEntity();

			m_Framebuffer->Unbind();
		}


	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHeirarchyPanel.SetScene(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Crane Scene (*.crane)\0*.crane\0");

		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		NewScene();

		SceneSerializer sceneSerializer(m_ActiveScene);
		sceneSerializer.Deserialize(path.string());
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::OpenFile("Crane Scene (*.crane)\0*.crane\0");

		if (!filepath.empty())
		{
			SceneSerializer sceneSerializer(m_ActiveScene);
			sceneSerializer.Serialize(filepath);
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
		m_ActiveScene->OnRuntimeStart();
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
		m_ActiveScene->OnRuntimeStop();
	}

	void EditorLayer::SelectEntity()
	{
		// Entity Selection Shenanigans
		ImVec2 globalMousePos = ImGui::GetMousePos();
		if (m_ViewportBounds[0].x > 0.0f)
			globalMousePos.x -= m_ViewportBounds[0].x;
		if (m_ViewportBounds[0].y > 0.0f)
			globalMousePos.y -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		globalMousePos.y = m_ViewportSize.y - globalMousePos.y;

		int mouseX = (int)globalMousePos.x;
		int mouseY = (int)globalMousePos.y;

		int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
		if (pixelData != -1)
			m_SceneHeirarchyPanel.SetSelectedEntity({ (entt::entity)pixelData, m_ActiveScene.get() });

		m_SelectEntity = false;
		
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0 , 2});
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {0 , 0});

		auto& colors = ImGui::GetStyle().Colors;
		ImGui::PushStyleColor(ImGuiCol_Button, colors[ImGuiCol_Button]);

		ImGui::Begin("##toolbar", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();

		// Center the button
		ImGui::SetCursorPosX(panelSize.x * 0.5f - panelSize.y);

		if (ImGui::ImageButton((ImTextureID)m_PlayButtonTexture->GetRendererID(), {panelSize.y, panelSize.y }, {0,0}, {1,1}, 0))
		{
			OnScenePlay();
		}

		ImGui::SameLine();

		if (ImGui::ImageButton((ImTextureID)m_StopButtonTexture->GetRendererID(), { panelSize.y, panelSize.y }, { 0,0 }, { 1,1 }, 0))
		{
			OnSceneStop();
		}

		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
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
		ImGuiStyle& style = ImGui::GetStyle();
		float WinMinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = WinMinSizeX;


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();


				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHeirarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::Separator();
		ImGui::Separator();

		ImGui::Text("Rendering Time: ");
		ImGui::Text("FPS: %.2f", 1/m_Timestep);

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		//CR_CORE_WARN("MinBounds: {0}, {1}, MaxBound: {2}, {3}", m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x, m_ViewportBounds[1].y );

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->AllowEvents(m_ViewportFocused || m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(uint64_t)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* relativePath = (wchar_t*)payload->Data;

				// if here we pass a dir or something that is not a .crane file this will crash
				OpenScene(g_AssetPath / relativePath);
			}

			ImGui::EndDragDropTarget();
		}


		// Gizmos
		Entity selectedEntity = m_SceneHeirarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoMode != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			
			// Camera Entity
			//Entity cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			//const SceneCamera& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
			//const glm::mat4& cameraProj = camera.GetProjection();

			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();

			// Selected Entity transform
			TransformComponent& entityTransformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 entityTransform = entityTransformComponent.GetTransform();
			
			// Snapping
			bool snapEnabled = Input::IsKeyPressed(Key::LeftControl);
			float snapAmount = 0.5f;
			if (m_GizmoMode == (int32_t)ImGuizmo::ROTATE)
				snapAmount = 45.0f;

			float snapAmountVec[3] = { snapAmount, snapAmount, snapAmount };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
				(ImGuizmo::OPERATION)m_GizmoMode, ImGuizmo::LOCAL, glm::value_ptr(entityTransform), (float*)0, snapEnabled ? snapAmountVec : (float*)0);
			

			// All of this could be skipped if we had a way to store transform matrix inside transformComponent
			if (ImGuizmo::IsUsing())
			{
				// but since we dont 
				glm::vec3 entityTranslation, entityRotation, entityScale;
				// we have to decompose the matrix we get back from ImGuizmo
				Math::DecomposeTransform(entityTransform, entityTranslation, entityRotation, entityScale);

				glm::vec3 deltaRotation = entityRotation - entityTransformComponent.Rotation;
				// do other shit to set trs 
				entityTransformComponent.Translation = entityTranslation;
				entityTransformComponent.Rotation += deltaRotation;
				entityTransformComponent.Scale = entityScale;
				
				// and then once we need to use the matrix 
				// we have to calculate it
			}
			// this seems bad

		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
		
	}
}