#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Crane
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) { m_Position = position; CalculateViewMatrix(); }
		void SetRotation(float rotation) { m_Rotation = rotation; CalculateViewMatrix(); }
		void SetProjMatrix(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

		inline const glm::mat4& GetViewProjMatrix() const { return m_ViewProjMatrix; }
	
	private:
		void CalculateViewMatrix();
		void CalculateViewProjMatrix();

	private:
		glm::mat4 m_ViewProjMatrix;
		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position;
		float m_Rotation = 0;
	};
}