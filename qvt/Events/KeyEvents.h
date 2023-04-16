#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H
#include "Events/Event.h"
#include "Core/KeyCodes.h"

namespace qvt
{
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode()const { return m_keyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(const KeyCode keycode) :m_keyCode(keycode) {}

		KeyCode m_keyCode;

	};

	class KeyPressedEvent : public KeyEvent
	{
	public:

		KeyPressedEvent(const KeyCode keycode, const uint16_t repeatcount) :KeyEvent(keycode), m_repeatCount(repeatcount) {}

		uint16_t GetRepeatCount()const { return m_repeatCount; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << "times);";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		uint16_t m_repeatCount;

	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:

		KeyReleasedEvent(const KeyCode keycode) :KeyEvent(keycode) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode) :KeyEvent(keycode) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}


#endif

