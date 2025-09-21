#include "Application.h"
#include "log/LogSystem.h"

namespace Cola
{
    Application::Application(WindowCreateInfo windowInfo)
    {
        mWindow = Cola::Window::Create(windowInfo.width, windowInfo.height, windowInfo.windowName);

        mWindow->SetEventCallback(std::bind(OnEvent, this, std::placeholders::_1));
    }
    
    Application::~Application()
    {
    }

    void Application::OnEvent(Event &e)
    {
        LOG_INFO("Application::OnEvent");
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(OnWindowClose, this, std::placeholders::_1));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        LOG_INFO("Application::OnWindowClose");
        mRunning = false;
        return true;
    }

    void Application::Run()
    {
        while (mRunning)
        {
            Frame();
        }
        
    }

    void Application::Frame()
    {
        mWindow->PollEvents();

        mWindow->SwapBuffers();
    }
}