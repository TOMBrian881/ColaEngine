#ifndef COLA_GLFW_WINDOW_H
#define COLA_GLFW_WINDOW_H
#include "../ColaWindow.h"
#include <glfw/glfw3.h>

namespace Cola
{
    class ColaGlfwWindow : public ColaWindow
    {
    public:
        ColaGlfwWindow() = delete;
        ColaGlfwWindow(uint32_t width, uint32_t height, const char* windowName);
        ~ColaGlfwWindow() override;

        virtual bool ShouldClose();
        virtual void PollEvents();
        virtual void SwapBuffers();
        GLFWwindow * getWindow();

    private:
        GLFWwindow *mGLFWwindow;
    };
}


#endif