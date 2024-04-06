#pragma once

#include "Crane/Renderer/OrthographicCamera.h"

#include "Crane/Core/Timestep.h"

#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/MouseEvent.h"

namespace Crane
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);

		inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		void CalculateView();
	private:
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		OrthographicCamera m_Camera;

		bool m_Rotation;
		float m_CameraRotation = 0;
		float m_CameraRotationSpeed = 0.2f;
		float m_PrevMouseX = 0;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraTranslationSpeed = 3.0f;
	};
}