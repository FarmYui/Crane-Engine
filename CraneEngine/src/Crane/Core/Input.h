#pragma once

#include "Crane/Core/Core.h"
#include "Crane/Core/KeyCodes.h"
#include "Crane/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Crane
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}