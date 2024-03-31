#pragma once

#include "Crane/Renderer/OrthographicCamera.h"

#include "Crane/Renderer/Texture.h"

namespace Crane
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();


		// ////////////////////
		// primitives
		// ////////////////////

		/* without rotation */
		//-- colored 
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color, float alpha = 1.0f);
		//-- textured
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec3& color = { 1.0f,1.0f,1.0f }, float alpha = 1.0f);

		/* with rotation */
		//-- colored 
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& color, float alpha = 1.0f);
		//-- textured
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec3& color = { 1.0f,1.0f,1.0f }, float alpha = 1.0f);

	};
}