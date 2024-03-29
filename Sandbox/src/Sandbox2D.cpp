#include "Sandbox2D.h"


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(Crane::OrthographicCameraController(1.78f, true)) 
{};

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
	
}

void Sandbox2D::OnEvent(Crane::Event& e)
{
	m_CameraController.OnEvent(e);
}

void Sandbox2D::OnUpdate(Crane::Timestep ts)
{
	m_Timestep = ts;

	m_CameraController.OnUpdate(ts);

	Crane::RenderCommand::SetClearColor(m_ClearColor);
	Crane::RenderCommand::Clear();
	
	Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Crane::Renderer2D::DrawQuad(m_QuadPosition, m_QuadSize, m_QuadColor);

	Crane::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Graphics Context");
	ImGui::Text("%s", Crane::Application::Get().GetWindow().GetContext()->GetInfo().c_str());
	ImGui::End();


	ImGui::Begin("Info");
	ImGui::Text("FPS: %.1f", 1.0f / m_Timestep);
	ImGui::Text("Last Frame: %.2fms", m_Timestep.GetMilliseconds());
	ImGui::End();


	ImGui::Begin("Scene");
	ImGui::DragFloat3("Quad Position", &m_QuadPosition[0], 0.01f);
	ImGui::DragFloat3("Quad Size", &m_QuadSize[0], 0.01f);
	ImGui::ColorEdit3("Quad Color", &m_QuadColor[0]);
	ImGui::End();

	ImGui::Begin("Ambient");
	ImGui::ColorEdit3("Clear Color", &m_ClearColor[0]);
	
	ImGui::End();

}
