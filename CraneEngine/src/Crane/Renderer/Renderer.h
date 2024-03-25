#pragma once

#include "RendererAPI.h"
#include "RendererCommand.h"

#include "Shader.h"
#include "OrthographicCamera.h"

namespace Crane
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);

		static void EndScene() {};

		static void Submit(const std::shared_ptr<VertexArray>& VertexArray, const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjMatrix;
		};

		static SceneData* s_SceneData;
	};
}