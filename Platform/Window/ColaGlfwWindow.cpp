#include "ColaGlfwWindow.h"
// #include "../../Core/log/LogSystem.h"
#include "log/LogSystem.h"

namespace Cola
{
    ColaGlfwWindow::ColaGlfwWindow(uint32_t width, uint32_t height, const char* windowName)
    {
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

    GLFWwindow *ColaGlfwWindow::getWindow()
    {
        return mGLFWwindow;
    }
}