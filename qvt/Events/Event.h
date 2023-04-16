#ifndef EVENT_H
#define EVENT_H
#include <vector>
#include <string>
#include <sstream>

#define BIT(x) (1<<x)

namespace qvt
{
    enum class EventType
    {
        None = 0,
        WindowResize,WindowClose,
        KeyPressed,KeyReleased,KeyTyped,
        MouseButtonPressed,MouseButtonReleased,
        MouseMoved,MouseScrolled

    };

    enum EventCategory
    {
        None = 0,
        EventCategoryInput = BIT(0),
        EventCategoryKeyboard = BIT(1),
        EventCategoryMouse = BIT(2),
        EventCategoryApplication = BIT(3),
        EventCategoryMouseButton = BIT(4)

    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::type;}\
                               virtual EventType GetEventType()const override {return GetStaticType();}\
                               virtual const char* GetName()const override {return #type;}


#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags()const override{return category;}

    class Event
    {
        public:
            Event() = default;
            Event(const Event& e) = default;
            virtual ~Event() = default;
            virtual EventType GetEventType()const = 0;
            virtual const char* GetName()const = 0;
            virtual int GetCategoryFlags()const = 0;
            virtual std::string ToString()const;
            bool IsInCategory(EventCategory);


        public:
            bool Handled = false;


        private:
            EventType  type;
            EventCategory category;

    };

    class EventHandler
    {
        public:
            EventHandler(Event& event);
            virtual ~EventHandler() = default;

            template<typename T,typename F>
            bool Handle(const F& func)
            {
                if(m_event.GetEventType()==T::GetStaticType())
                {
                    m_event.Handled |=  func(static_cast<T&>(m_event));

                    return true;
                }

                return false;
            }


        private:
            Event& m_event;

    };

    inline std::ostream& operator<<(std::ostream& os,const Event& e)
    {
        return os << e.ToString();
    }

}
#endif