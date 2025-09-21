#ifndef EVENT_H
#define EVENT_H

namespace Cola
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize
    };

    class Event
    {
    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;

    
        bool mHandled = false;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
        :mEvent(event)
        {

        }
        ~EventDispatcher();

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            bool result = false;
            if (mEvent.GetEventType() == T::GetStaticType())
            {
                mEvent.mHandled = func(static_cast<T&>(mEvent));
                result = true;
            }

            return result;
        }
    private:
        Event &mEvent;
    };
    

}
#endif