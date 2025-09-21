#include "GraphicRhi.h"
#include "vulkanRhi/VulkanRHI.h"

namespace Cola
{
    std::unique_ptr<GraphicRhi> GraphicRhi::Create(Window *window)
    {
#ifdef COLA_ENGINE_GRAPHIC_API_VULKAN
        return std::make_unique<VulkanRHI>(window);
#else
        return nullptr;
#endif
    }
}