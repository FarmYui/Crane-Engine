#pragma once

#include "Crane/Core/Input.h"

#include <glm/glm.hpp>

namespace Crane
{
	class CRANE_API WindowsInput : public Input
	{
	private:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual glm::vec2 GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	};
}