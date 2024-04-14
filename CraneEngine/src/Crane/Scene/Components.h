#pragma once

#include "glm/glm.hpp"

#include "Crane/Renderer/Camera.h"

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
		Crane::Camera Camera;
		bool Primary = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}

		operator const Crane::Camera& () const { return Camera; }
		operator Crane::Camera& () { return Camera; }

	};

}