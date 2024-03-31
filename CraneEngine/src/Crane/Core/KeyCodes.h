#pragma once

namespace Crane
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}
#define CR_KEY_SPACE           ::Crane::Key::Space
#define CR_KEY_APOSTROPHE      ::Crane::Key::Apostrophe    /* ' */
#define CR_KEY_COMMA           ::Crane::Key::Comma         /* , */
#define CR_KEY_MINUS           ::Crane::Key::Minus         /* - */
#define CR_KEY_PERIOD          ::Crane::Key::Period        /* . */
#define CR_KEY_SLASH           ::Crane::Key::Slash         /* / */
#define CR_KEY_0               ::Crane::Key::D0
#define CR_KEY_1               ::Crane::Key::D1
#define CR_KEY_2               ::Crane::Key::D2
#define CR_KEY_3               ::Crane::Key::D3
#define CR_KEY_4               ::Crane::Key::D4
#define CR_KEY_5               ::Crane::Key::D5
#define CR_KEY_6               ::Crane::Key::D6
#define CR_KEY_7               ::Crane::Key::D7
#define CR_KEY_8               ::Crane::Key::D8
#define CR_KEY_9               ::Crane::Key::D9
#define CR_KEY_SEMICOLON       ::Crane::Key::Semicolon     /* ; */
#define CR_KEY_EQUAL           ::Crane::Key::Equal         /* = */
#define CR_KEY_A               ::Crane::Key::A
#define CR_KEY_B               ::Crane::Key::B
#define CR_KEY_C               ::Crane::Key::C
#define CR_KEY_D               ::Crane::Key::D
#define CR_KEY_E               ::Crane::Key::E
#define CR_KEY_F               ::Crane::Key::F
#define CR_KEY_G               ::Crane::Key::G
#define CR_KEY_H               ::Crane::Key::H
#define CR_KEY_I               ::Crane::Key::I
#define CR_KEY_J               ::Crane::Key::J
#define CR_KEY_K               ::Crane::Key::K
#define CR_KEY_L               ::Crane::Key::L
#define CR_KEY_M               ::Crane::Key::M
#define CR_KEY_N               ::Crane::Key::N
#define CR_KEY_O               ::Crane::Key::O
#define CR_KEY_P               ::Crane::Key::P
#define CR_KEY_Q               ::Crane::Key::Q
#define CR_KEY_R               ::Crane::Key::R
#define CR_KEY_S               ::Crane::Key::S
#define CR_KEY_T               ::Crane::Key::T
#define CR_KEY_U               ::Crane::Key::U
#define CR_KEY_V               ::Crane::Key::V
#define CR_KEY_W               ::Crane::Key::W
#define CR_KEY_X               ::Crane::Key::X
#define CR_KEY_Y               ::Crane::Key::Y
#define CR_KEY_Z               ::Crane::Key::Z
#define CR_KEY_LEFT_BRACKET    ::Crane::Key::LeftBracket   /* [ */
#define CR_KEY_BACKSLASH       ::Crane::Key::Backslash     /* \ */
#define CR_KEY_RIGHT_BRACKET   ::Crane::Key::RightBracket  /* ] */
#define CR_KEY_GRAVE_ACCENT    ::Crane::Key::GraveAccent   /* ` */
#define CR_KEY_WORLD_1         ::Crane::Key::World1        /* non-US #1 */
#define CR_KEY_WORLD_2         ::Crane::Key::World2        /* non-US #2 */

/* Function keys */
#define CR_KEY_ESCAPE          ::Crane::Key::Escape
#define CR_KEY_ENTER           ::Crane::Key::Enter
#define CR_KEY_TAB             ::Crane::Key::Tab
#define CR_KEY_BACKSPACE       ::Crane::Key::Backspace
#define CR_KEY_INSERT          ::Crane::Key::Insert
#define CR_KEY_DELETE          ::Crane::Key::Delete
#define CR_KEY_RIGHT           ::Crane::Key::Right
#define CR_KEY_LEFT            ::Crane::Key::Left
#define CR_KEY_DOWN            ::Crane::Key::Down
#define CR_KEY_UP              ::Crane::Key::Up
#define CR_KEY_PAGE_UP         ::Crane::Key::PageUp
#define CR_KEY_PAGE_DOWN       ::Crane::Key::PageDown
#define CR_KEY_HOME            ::Crane::Key::Home
#define CR_KEY_END             ::Crane::Key::End
#define CR_KEY_CAPS_LOCK       ::Crane::Key::CapsLock
#define CR_KEY_SCROLL_LOCK     ::Crane::Key::ScrollLock
#define CR_KEY_NUM_LOCK        ::Crane::Key::NumLock
#define CR_KEY_PRINT_SCREEN    ::Crane::Key::PrintScreen
#define CR_KEY_PAUSE           ::Crane::Key::Pause
#define CR_KEY_F1              ::Crane::Key::F1
#define CR_KEY_F2              ::Crane::Key::F2
#define CR_KEY_F3              ::Crane::Key::F3
#define CR_KEY_F4              ::Crane::Key::F4
#define CR_KEY_F5              ::Crane::Key::F5
#define CR_KEY_F6              ::Crane::Key::F6
#define CR_KEY_F7              ::Crane::Key::F7
#define CR_KEY_F8              ::Crane::Key::F8
#define CR_KEY_F9              ::Crane::Key::F9
#define CR_KEY_F10             ::Crane::Key::F10
#define CR_KEY_F11             ::Crane::Key::F11
#define CR_KEY_F12             ::Crane::Key::F12
#define CR_KEY_F13             ::Crane::Key::F13
#define CR_KEY_F14             ::Crane::Key::F14
#define CR_KEY_F15             ::Crane::Key::F15
#define CR_KEY_F16             ::Crane::Key::F16
#define CR_KEY_F17             ::Crane::Key::F17
#define CR_KEY_F18             ::Crane::Key::F18
#define CR_KEY_F19             ::Crane::Key::F19
#define CR_KEY_F20             ::Crane::Key::F20
#define CR_KEY_F21             ::Crane::Key::F21
#define CR_KEY_F22             ::Crane::Key::F22
#define CR_KEY_F23             ::Crane::Key::F23
#define CR_KEY_F24             ::Crane::Key::F24
#define CR_KEY_F25             ::Crane::Key::F25
		
/* Keypad*/
#define CR_KEY_KP_0            ::Crane::Key::KP0
#define CR_KEY_KP_1            ::Crane::Key::KP1
#define CR_KEY_KP_2            ::Crane::Key::KP2
#define CR_KEY_KP_3            ::Crane::Key::KP3
#define CR_KEY_KP_4            ::Crane::Key::KP4
#define CR_KEY_KP_5            ::Crane::Key::KP5
#define CR_KEY_KP_6            ::Crane::Key::KP6
#define CR_KEY_KP_7            ::Crane::Key::KP7
#define CR_KEY_KP_8            ::Crane::Key::KP8
#define CR_KEY_KP_9            ::Crane::Key::KP9
#define CR_KEY_KP_DECIMAL      ::Crane::Key::KPDecimal
#define CR_KEY_KP_DIVIDE       ::Crane::Key::KPDivide
#define CR_KEY_KP_MULTIPLY     ::Crane::Key::KPMultiply
#define CR_KEY_KP_SUBTRACT     ::Crane::Key::KPSubtract
#define CR_KEY_KP_ADD          ::Crane::Key::KPAdd
#define CR_KEY_KP_ENTER        ::Crane::Key::KPEnter
#define CR_KEY_KP_EQUAL        ::Crane::Key::KPEqual
		
#define CR_KEY_LEFT_SHIFT      ::Crane::Key::LeftShift
#define CR_KEY_LEFT_CONTROL    ::Crane::Key::LeftControl
#define CR_KEY_LEFT_ALT        ::Crane::Key::LeftAlt
#define CR_KEY_LEFT_SUPER      ::Crane::Key::LeftSuper
#define CR_KEY_RIGHT_SHIFT     ::Crane::Key::RightShift
#define CR_KEY_RIGHT_CONTROL   ::Crane::Key::RightControl
#define CR_KEY_RIGHT_ALT       ::Crane::Key::RightAlt
#define CR_KEY_RIGHT_SUPER     ::Crane::Key::RightSuper
#define CR_KEY_MENU            ::Crane::Key::Menu