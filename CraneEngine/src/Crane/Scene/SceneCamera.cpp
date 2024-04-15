#include "crpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Crane
{

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		
		CalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		
		CalculateProjection();
	}

	void SceneCamera::CalculateProjection()
	{
		m_Projection = glm::ortho(m_OrthographicSize * m_AspectRatio * 0.5f, -m_OrthographicSize * m_AspectRatio * 0.5f, m_OrthographicSize * 0.5f, -m_OrthographicSize * 0.5f, m_OrthographicNear, m_OrthographicFar);
	}
}