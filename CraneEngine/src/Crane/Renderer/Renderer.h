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

		static void Submit(const Ref<VertexArray>& VertexArray, const Ref<Shader>& shader, const glm::mat4& transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjMatrix;
		};

		static SceneData* s_SceneData;
	};
}