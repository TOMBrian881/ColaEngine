#ifndef APPLICATION_H
#define APPLICATION_H
#include "ColaStart.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Cola
{
    struct WindowCreateInfo
    {
        uint32_t width {1280};
        uint32_t height {720};
        const char* windowName {"ColaEngine"};
    };

    class Application
    {
    public:
        Application(WindowCreateInfo windowInfo);
        ~Application();

        void OnEvent(Event &e);

        void Run();
        void Frame();
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        std::unique_ptr<Cola::Window> mWindow;
        bool mRunning = true;
    };
    
} // Cola
#endif