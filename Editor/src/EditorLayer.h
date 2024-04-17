#pragma once
#include "Crane.h"

#include "Panels/SceneHierarchyPanel.h"

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
		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		Timestep m_Timestep;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;

		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;
		bool m_PrimaryCamera = true;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize;


		// Panels
		SceneHierarchyPanel m_SceneHeirarchyPanel;
	};
}