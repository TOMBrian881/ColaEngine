#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H
#include "Event.h"

namespace Cola
{
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        static EventType GetStaticType() { return EventType::WindowClose;}
        virtual EventType GetEventType() const override {return GetStaticType();}
        virtual const char* GetName() const override {return "WindowCloseEvent";}
    };
    
}

#endif