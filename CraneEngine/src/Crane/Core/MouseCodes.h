#pragma once

namespace Crane
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CR_MOUSE_BUTTON_0      ::Crane::Mouse::Button0
#define CR_MOUSE_BUTTON_1      ::Crane::Mouse::Button1
#define CR_MOUSE_BUTTON_2      ::Crane::Mouse::Button2
#define CR_MOUSE_BUTTON_3      ::Crane::Mouse::Button3
#define CR_MOUSE_BUTTON_4      ::Crane::Mouse::Button4
#define CR_MOUSE_BUTTON_5      ::Crane::Mouse::Button5
#define CR_MOUSE_BUTTON_6      ::Crane::Mouse::Button6
#define CR_MOUSE_BUTTON_7      ::Crane::Mouse::Button7
#define CR_MOUSE_BUTTON_LAST   ::Crane::Mouse::ButtonLast
#define CR_MOUSE_BUTTON_LEFT   ::Crane::Mouse::ButtonLeft
#define CR_MOUSE_BUTTON_RIGHT  ::Crane::Mouse::ButtonRight
#define CR_MOUSE_BUTTON_MIDDLE ::Crane::Mouse::ButtonMiddle