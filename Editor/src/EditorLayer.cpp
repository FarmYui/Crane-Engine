#include "EditorLayer.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Crane
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{};

	void EditorLayer::OnAttach()
	{
		CR_PROFILE_FUNCTION();

		FramebufferSpecification specification;
		specification.Width = 1280;
		specification.Height = 720;
		m_Framebuffer = Framebuffer::Create(specification);

		m_ActiveScene = CreateRef<Scene>();

		Entity quad = m_ActiveScene->CreateEntity("Quad A");
		quad.AddComponent<SpriteRendererComponent>(glm::vec4(0.1f, 0.2f, 1.0f, 0.7f));
		
		Entity quadb = m_ActiveScene->CreateEntity("Quad B");
		quadb.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
		

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>().Primary = true;

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("Second Camera Entity");
		m_SecondCameraEntity.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		private:
			float m_CameraSpeed = 5.0f;
		public:

			virtual void OnUpdate(Timestep ts) override
			{
				glm::vec3& cameraTranslation = GetComponent<TransformComponent>().Translation;
				
				if (Input::IsKeyPressed(Key::W))
					cameraTranslation.y += m_CameraSpeed * ts;

				if (Input::IsKeyPressed(Key::S))
					cameraTranslation.y -= m_CameraSpeed * ts;

				if (Input::IsKeyPressed(Key::A))
					cameraTranslation.x -= m_CameraSpeed * ts;

				if (Input::IsKeyPressed(Key::D))
					cameraTranslation.x += m_CameraSpeed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHeirarchyPanel.SetScene(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		CR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnEvent(Event& e)
	{
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
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

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

			m_ActiveScene->OnUpdate(ts);

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

		m_SceneHeirarchyPanel.OnImGuiRender();
		
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

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->AllowEvents(m_ViewportFocused && m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(uint64_t)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
		
	}
}