#include "crpch.h"
#include "Renderer.h"

namespace Crane
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjMatrix = camera.GetViewProjMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& VertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f))
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewProjMatrix);
		shader->SetUniformMat4("u_Model", transform);

		VertexArray->Bind();
		RendererCommand::DrawIndexed(VertexArray);
	}
}