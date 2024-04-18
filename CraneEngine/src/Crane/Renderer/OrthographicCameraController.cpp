#include "crpch.h"
#include "OrthographicCameraController.h"

#include "Crane/Core/Core.h"
#include "Crane/Core/Input.h"
#include "Crane/Core/KeyCodes.h"
#include "Crane/Core/MouseCodes.h"

namespace Crane
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		CR_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(CR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(CR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		CR_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation + 90)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation + 90)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation + 90)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation + 90)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(Key::A))
		{
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsMouseButtonPressed(Mouse::Button2))
			{
				float currentMouseX = Input::GetMouseX();
				m_CameraRotation += (m_PrevMouseX) ? ((currentMouseX - m_PrevMouseX) * m_CameraRotationSpeed) : 0;
				m_PrevMouseX = currentMouseX;
			}
			else
			{
				m_PrevMouseX = 0;
			}
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 2.0f;
		
	}

	void OrthographicCameraController::CalculateProj()
	{
		m_Camera.SetProjMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateProj();
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		CR_PROFILE_FUNCTION();
		Resize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		CR_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * 0.15f * m_ZoomLevel;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateProj();
		return false;
	}
	
}