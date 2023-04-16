#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H
#include "Event.h"
#include "Core/MouseCodes.h"

namespace qvt
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y) :m_mouseX(x), m_mouseY(y) {}
		float GetX()const { return m_mouseX; }
		float GetY()const { return m_mouseY; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_mouseX;
		float m_mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xoffset, const float yoffset) :x_offset(xoffset), y_offset(yoffset) {}

		float GetXOffset()const { return x_offset; }
		float GetYOffset()const { return y_offset; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << x_offset << ", " << y_offset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float x_offset;
		float y_offset;
	};


	class MouseEvent : public Event
	{
	public:
		MouseCode GetMouseButton()const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

	protected:
		MouseEvent(const MouseCode mousecode) :m_button(mousecode) {}
		MouseCode m_button;
	};


	class MouseButtonPressedEvent : public MouseEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode mousecode) :MouseEvent(mousecode) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode mousecode) :MouseEvent(mousecode) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}


#endif

