#ifndef VULKAN_TOOL_H
#define VULKAN_TOOL_H
#include "ColaStart.h"
#include <vulkan/vulkan.h>


namespace Cola
{
    namespace VulkanTool
    {
        VkShaderModule loadShader(const std::string filePath, const VkDevice &device);
    }
}

#endif