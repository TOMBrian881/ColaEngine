#ifndef COLA_WINDOW_H
#define COLA_WINDOW_H
#include "ColaStart.h"
#include "Events/Event.h"
#include <GLFW/glfw3.h>
namespace Cola
{
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;
        Window() = default;
        Window(const Window&) = delete;
        Window &operator=(const Window&) = delete;
        virtual ~Window() = default;

        virtual bool ShouldClose() = 0;
        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual const std::string& GetTitle() const = 0;
        virtual void SetTitle(const std::string& title) = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        virtual void* getWindow() const = 0;

        static std::unique_ptr<Window> Create(uint32_t width, uint32_t height, const char* windowName);
    };
}

#endif