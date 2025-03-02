#ifndef VK_COMMON_H
#define VK_COMMON_H
#include <vulkan/vulkan.h>
#include "ColaStart.h"
namespace Cola
{
    struct QueueFamilyInfo
    {
        int mGraphicIndex = -1;
        int mComputerIndex = -1;
        int mPresentIndex = -1;
    };

    struct SwapChainSupportDetail
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formatVec;
        std::vector<VkPresentModeKHR> presentModeVec;
    };
    
}
#endif