#ifndef COLA_WINDOW_H
#define COLA_WINDOW_H
#include "ColaStart.h"
#include <GLFW/glfw3.h>
namespace Cola
{
    class ColaWindow
    {
    public:
        ColaWindow() = default;
        ColaWindow(const ColaWindow&) = delete;
        ColaWindow &operator=(const ColaWindow&) = delete;
        virtual ~ColaWindow() = default;

        virtual bool ShouldClose() = 0;
        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;
        virtual GLFWwindow * getWindow() = 0;

        static std::unique_ptr<ColaWindow> Create(uint32_t width, uint32_t height, const char* windowName);
    };
}

#endif