#include "Window.h"
#include "Window/ColaGlfwWindow.h"

namespace Cola
{
    std::unique_ptr<Window> Window::Create(uint32_t width, uint32_t height, const char* windowName)
    {
#ifdef ENGINE_PLATFORM_WIN32
        return std::make_unique<ColaGlfwWindow>(width, height, windowName);
#elif ENGINE_PLATFORM_MACOS
        return std::make_unique<ColaGlfwWindow>(width, height, windowName);
#elif ENGINE_PLATFORM_LINUX
        return std::make_unique<ColaGlfwWindow>(width, height, windowName);
#endif
    }
}