#pragma once

namespace Crane
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button1 = 0,
		Button2 = 1,
		Button3 = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,

		ButtonLast = Button8,
		ButtonLeft = Button1,
		ButtonRight = Button2,
		ButtonMiddle = Button3
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CR_MOUSE_BUTTON_1      ::Crane::Mouse::Button1
#define CR_MOUSE_BUTTON_2      ::Crane::Mouse::Button2
#define CR_MOUSE_BUTTON_3      ::Crane::Mouse::Button3
#define CR_MOUSE_BUTTON_4      ::Crane::Mouse::Button4
#define CR_MOUSE_BUTTON_5      ::Crane::Mouse::Button5
#define CR_MOUSE_BUTTON_6      ::Crane::Mouse::Button6
#define CR_MOUSE_BUTTON_7      ::Crane::Mouse::Button7
#define CR_MOUSE_BUTTON_8      ::Crane::Mouse::Button8
#define CR_MOUSE_BUTTON_LAST   ::Crane::Mouse::ButtonLast
#define CR_MOUSE_BUTTON_LEFT   ::Crane::Mouse::ButtonLeft
#define CR_MOUSE_BUTTON_RIGHT  ::Crane::Mouse::ButtonRight
#define CR_MOUSE_BUTTON_MIDDLE ::Crane::Mouse::ButtonMiddle