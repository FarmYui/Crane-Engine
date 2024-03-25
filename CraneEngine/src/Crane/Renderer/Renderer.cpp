#include "crpch.h"
#include "Renderer.h"

namespace Crane
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjMatrix = camera.GetViewProjMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& VertexArray, const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", m_SceneData->ViewProjMatrix);

		VertexArray->Bind();
		RendererCommand::DrawIndexed(VertexArray);
	}
}