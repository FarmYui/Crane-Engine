#pragma once

#include "glm/glm.hpp"

#include "Crane/Scene/SceneCamera.h"

namespace Crane
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator const std::string& () const { return Tag; }
		operator std::string& () { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& tranform)
			: Transform(tranform) {}

		operator const glm::mat4& () const { return Transform; }
		operator glm::mat4& () { return Transform; }

	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		//CameraComponent(uint32_t width, uint32_t height, float size, float nearClip, float farClip)
		//	: Camera(width,height, size, nearClip, farClip) {}

		operator const SceneCamera& () const { return Camera; }
		operator SceneCamera& () { return Camera; }

	};

}