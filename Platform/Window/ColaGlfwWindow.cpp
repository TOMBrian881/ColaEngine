#include "ColaGlfwWindow.h"
// #include "../../Core/log/LogSystem.h"
#include "log/LogSystem.h"
#include "Events/ApplicationEvent.h"

namespace Cola
{
    ColaGlfwWindow::ColaGlfwWindow(uint32_t width, uint32_t height, const char* windowName)
    {
        mWindowData.Width = width;
        mWindowData.Height = height;
        mWindowData.Title = windowName;

        LOG_INFO("Create window {0}: width = {1}, height = {2}", windowName, width, height);

        if (!glfwInit())
        {
            LOG_ERROR("failed to init GLFW");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE, false);
        mGLFWwindow = glfwCreateWindow(width, height, windowName, nullptr, nullptr);

        if (!mGLFWwindow)
        {
            LOG_ERROR("failed to create window");
            glfwTerminate();
            return;
        }

        glfwSetWindowUserPointer(mGLFWwindow, &mWindowData);


        glfwSetWindowCloseCallback(mGLFWwindow, [](GLFWwindow* window)
        {
            LOG_INFO("WindowCloseCallback");
            WindowDate& data = *(WindowDate *)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        
        if (primaryMonitor)
        {
            int xpos, ypos, screenWidth, screenHeight;
            glfwGetMonitorWorkarea(primaryMonitor, &xpos, &ypos, &screenWidth, &screenHeight);
            glfwSetWindowPos(mGLFWwindow, (screenWidth - width) / 2, (screenHeight - height) / 2);
        }

        glfwMakeContextCurrent(mGLFWwindow);
        glfwShowWindow(mGLFWwindow);
    }

    ColaGlfwWindow::~ColaGlfwWindow()
    {
        glfwDestroyWindow(mGLFWwindow);
        glfwTerminate();
    }

    bool ColaGlfwWindow::ShouldClose()
    {
        return glfwWindowShouldClose(mGLFWwindow);
    }

    void ColaGlfwWindow::PollEvents()
    {
        glfwPollEvents();
    }
    
    void ColaGlfwWindow::SwapBuffers()
    {
        glfwSwapBuffers(mGLFWwindow);
    }

    void* ColaGlfwWindow::getWindow() const
    {
        return mGLFWwindow;
    }
}