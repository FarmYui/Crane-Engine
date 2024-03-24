#include "crpch.h"
#include "Renderer.h"

namespace Crane
{
	void Renderer::Submit(const std::shared_ptr<VertexArray>& VertexArray)
	{
		VertexArray->Bind();
		RendererCommand::DrawIndexed(VertexArray);
	}
}