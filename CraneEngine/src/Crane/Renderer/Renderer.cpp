#include "crpch.h"
#include "Renderer.h"

namespace Crane
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjMatrix = camera.GetViewProjMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& VertexArray, const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewProjMatrix);

		VertexArray->Bind();
		RendererCommand::DrawIndexed(VertexArray);
	}
}