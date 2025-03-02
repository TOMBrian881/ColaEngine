#ifndef GRAPHIC_RHI_H
#define GRAPHIC_RHI_H
#define GLFW_INCLUDE_VULKAN
#include "ColaStart.h"
#include <GLFW/glfw3.h>

namespace Cola
{
    class ColaWindow;
    class GraphicRhi
    {
    public:
        GraphicRhi(const GraphicRhi&) = delete;
        GraphicRhi &operator=(const GraphicRhi&) = delete;
        virtual ~GraphicRhi() = default;

        static std::unique_ptr<GraphicRhi> Create(ColaWindow *window);
    protected:
        GraphicRhi() = default;
    };
}

#endif