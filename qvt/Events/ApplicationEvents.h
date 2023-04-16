#ifndef APPLICATION_EVENTS_H
#define APPLICATINO_EVENTS_H
#include "Event.h"
#include <sstream>

namespace qvt
{
    class WindowResizedEvent : public Event
    {
        public:
            WindowResizedEvent(uint32_t width,uint32_t height)
            {
                m_width = width;
                m_height = height;
                
            }

            EVENT_CLASS_TYPE(WindowResize)
            EVENT_CLASS_CATEGORY(EventCategoryApplication)

            uint32_t GetWidth()const { return m_width;}
            uint32_t GetHeight()const {return m_height;}

        private:
            uint32_t m_width;
            uint32_t m_height;

    };

    class WindowCloseEvent : public Event
    {
        public:
            WindowCloseEvent() = default;
            EVENT_CLASS_TYPE(WindowClose);
            EVENT_CLASS_CATEGORY(EventCategoryApplication);

    };
}



#endif