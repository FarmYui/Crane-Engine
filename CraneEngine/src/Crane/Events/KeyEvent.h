#pragma once

#include "Event.h"

#include <sstream>


namespace Crane
{
	class CRANE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) 
		{}
		
		int m_KeyCode;
	};

	class CRANE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatedCount)
			: KeyEvent(keycode), m_RepeatedCount(repeatedCount)
		{}

		inline int GetRepeatCount() const { return m_RepeatedCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatedCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatedCount;
	};

	class CRANE_API KeyRelasedEvent : public KeyEvent
	{
	public:
		KeyRelasedEvent(int keycode)
			: KeyEvent(keycode)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyRelasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyRelased)
	};
}