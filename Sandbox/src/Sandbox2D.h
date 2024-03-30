#pragma once
#include "Crane.h"

class Sandbox2D : public Crane::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnEvent(Crane::Event& e) override;
	void OnUpdate(Crane::Timestep ts) override;
	void OnImGuiRender() override;

private:
	glm::vec3 m_QuadPosition = { 0.0f, 0.0f, 0.0f };
	float m_QuadRotation = 0.0f;
	glm::vec3 m_QuadSize = { 1.0f, 1.0f, 1.0f };
	glm::vec3 m_QuadColor = { 1.0f, 0.0f, 1.0f };

	Crane::Ref<Crane::Texture2D> m_Texture;

	glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

	Crane::Timestep m_Timestep;

	Crane::OrthographicCameraController m_CameraController;
};