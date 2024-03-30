#pragma once

#include "Crane/Renderer/RenderCommand.h"

#include "Crane/Renderer/Shader.h"
#include "Crane/Renderer/OrthographicCamera.h"

namespace Crane
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera);

		static void EndScene() {};

		static void Submit(const Ref<VertexArray>& VertexArray, const Ref<Shader>& shader, const glm::mat4& transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}