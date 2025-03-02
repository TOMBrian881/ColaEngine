#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H
#include "ColaStart.h"
#include "VulkanShader.h"
#include <vulkan/vulkan.h>

namespace Cola
{
    class VulkanPipeline
    {
    public:
        VulkanPipeline(VkDevice *dvice, std::vector<std::shared_ptr<Cola::VulkanShader>> &shaderVec);
        ~VulkanPipeline();

        void init();

    private:
        VkDevice mDevice;
        std::vector<std::shared_ptr<Cola::VulkanShader>> mShaderVec;
    };

}
#endif