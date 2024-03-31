#pragma once

#include "Crane/Core/Input.h"

#include <glm/glm.hpp>

namespace Crane
{
	class WindowsInput : public Input
	{
	private:
		virtual bool IsKeyPressedImpl(KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual glm::vec2 GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	};
}