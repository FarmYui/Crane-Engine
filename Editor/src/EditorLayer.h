#pragma once
#include "Crane.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Crane/Renderer/EditorCamera.h"

namespace Crane
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event& e) override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	private:
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		Timestep m_Timestep;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;

		Entity m_HoveredEntity;

		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize{ 0.0f,0.0f };
		glm::vec2 m_ViewportBounds[2];

		// Panels
		SceneHierarchyPanel m_SceneHeirarchyPanel;

		int32_t m_GizmoMode = 0;
		float m_SnapAmount = 1.f;
	};
}