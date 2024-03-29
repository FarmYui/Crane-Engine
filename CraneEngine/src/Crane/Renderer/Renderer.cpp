#include "crpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Platforms/OpenGL/OpenGLShader.h"

namespace Crane
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_SceneData->ViewProjMatrix = camera.GetViewProjMatrix();
	}

	void Renderer::Submit(const Ref<VertexArray>& VertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f))
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewProjMatrix);
		shader->SetUniformMat4("u_Model", transform);

		VertexArray->Bind();
		RenderCommand::DrawIndexed(VertexArray);
	}
}