#include "Event.h"

namespace qvt
{
    std::string Event::ToString()const
    {
        return GetName();
    }

    bool Event::IsInCategory(EventCategory c)
    {
        return GetCategoryFlags() & c;
    }

    EventHandler::EventHandler(Event& event):m_event(event)
    {
        
    }
}