#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H
#include "ColaStart.h"
#include <vulkan/vulkan.h>

namespace Cola
{
    class VulkanShader
    {
    public:
        enum ShaderType
        {
            VERTEX = 0,
            FRAGMENT
        };
    
        VulkanShader(VkDevice *device, const std::string &filePath, ShaderType type);
        ~VulkanShader();

        VkShaderModule getShaderModule();
        VkShaderStageFlagBits getStageFlagBits();
    private:
        VkDevice *mDevice;
        ShaderType mType;
        VkShaderModule mShaderModule;
    };
}
#endif