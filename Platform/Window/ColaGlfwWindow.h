#ifndef COLA_GLFW_WINDOW_H
#define COLA_GLFW_WINDOW_H
#include "../Window.h"
#include <glfw/glfw3.h>

namespace Cola
{
    class ColaGlfwWindow : public Window
    {
    public:
        ColaGlfwWindow() = delete;
        ColaGlfwWindow(uint32_t width, uint32_t height, const char* windowName);
        ~ColaGlfwWindow() override;

        virtual bool ShouldClose() override;
        virtual void PollEvents() override;
        virtual void SwapBuffers() override;

        virtual uint32_t GetWidth() const override {return mWindowData.Width;}
        virtual uint32_t GetHeight() const override {return mWindowData.Height;}
        virtual const std::string& GetTitle() const override {return mWindowData.Title;}
        virtual void SetTitle(const std::string& title) override { mWindowData.Title = title;}
        virtual void SetEventCallback(const EventCallbackFn& callback) override { mWindowData.EventCallback = callback;}

        virtual void* getWindow() const override;

    private:
        GLFWwindow *mGLFWwindow;

        struct WindowDate
        {
            std::string Title;
            uint32_t Width;
            uint32_t Height;

            EventCallbackFn EventCallback;
        };

        WindowDate mWindowData;
    };
}


#endif