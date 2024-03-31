#include "Sandbox2D.h"
#include <imgui/imgui.h>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(Crane::OrthographicCameraController(1.78f, true)) 
{};

void Sandbox2D::OnAttach()
{
	CR_PROFILE_FUNCTION();

	m_Texture = Crane::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	CR_PROFILE_FUNCTION();	
}

void Sandbox2D::OnEvent(Crane::Event& e)
{
	m_CameraController.OnEvent(e);
}

void Sandbox2D::OnUpdate(Crane::Timestep ts)
{
	CR_PROFILE_FUNCTION();
	m_Timestep = ts;

	m_CameraController.OnUpdate(ts);

	// Clear
	{
		CR_PROFILE_SCOPE("Renderer Clear");
		Crane::RenderCommand::SetClearColor(m_ClearColor);
		Crane::RenderCommand::Clear();
	}

	// Draw
	{
		CR_PROFILE_SCOPE("Renderer Draw");
		Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Crane::Renderer2D::DrawQuad(m_QuadPosition, m_QuadSize, m_QuadColor, 1.0f, m_QuadRotation);
		Crane::Renderer2D::DrawQuad(glm::vec3(0.0f,0.0f, -0.1f), glm::vec3(10.0f), m_Texture);

		Crane::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	CR_PROFILE_FUNCTION();

	ImGui::Begin("Info");
	ImGui::Text("FPS: %.1f", 1.0f / m_Timestep);
	ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
	ImGui::End();


	ImGui::Begin("Scene");
	ImGui::DragFloat3("Quad Position", &m_QuadPosition[0], 0.01f);
	ImGui::DragFloat("Quad Rotation", &m_QuadRotation);
	ImGui::DragFloat3("Quad Size", &m_QuadSize[0], 0.01f);
	ImGui::ColorEdit3("Quad Color", &m_QuadColor[0]);
	ImGui::End();

	ImGui::Begin("Ambient");
	ImGui::ColorEdit3("Clear Color", &m_ClearColor[0]);
	
	ImGui::End();

}
