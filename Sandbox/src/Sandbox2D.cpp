#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <chrono>

template <typename Fn>
class Timer
{
public:
	Timer(/*const char* name, */Fn&& func)
		: /*m_Name(name),*/ m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();
	
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;


		m_Func(duration);
	}

private:
	//const char* m_Name;
	std::chrono::steady_clock::time_point m_StartTimepoint;
	Fn m_Func;
	bool m_Stopped;
};

#define CR_PROFILE_SCOPE(name) Timer timer##__LINE__([&](float duration) { m_ProfileResults.push_back({name,duration}); })

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(Crane::OrthographicCameraController(1.78f, true)) 
{};

void Sandbox2D::OnAttach()
{
	m_Texture = Crane::Texture2D::Create("assets/textures/Checkerboard.png");
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
	CR_PROFILE_SCOPE("Sandbox2D::OnUpdate");
	m_Timestep = ts;

	// Camera Controller
	{
		CR_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Clear
	{
		CR_PROFILE_SCOPE("Renderer::Clear");
		Crane::RenderCommand::SetClearColor(m_ClearColor);
		Crane::RenderCommand::Clear();
	}

	// Draw
	{
		CR_PROFILE_SCOPE("Renderer::Draw");
		Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Crane::Renderer2D::DrawQuad(m_QuadPosition, m_QuadSize, m_QuadColor, 1.0f, m_QuadRotation);
		Crane::Renderer2D::DrawQuad(glm::vec3(0.0f,0.0f, -0.1f), glm::vec3(10.0f), m_Texture);

		Crane::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Graphics Context");
	ImGui::Text("%s", Crane::Application::Get().GetWindow().GetContext()->GetInfo().c_str());
	ImGui::End();

	ImGui::Begin("Profiler");
	for (auto& result: m_ProfileResults)
		ImGui::Text("%.3fms : %s", result.Time, result.Name);
	m_ProfileResults.clear();
	ImGui::End();

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
