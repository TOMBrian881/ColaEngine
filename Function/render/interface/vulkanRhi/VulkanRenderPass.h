#ifndef VULKAN_RENDER_PASS
#define VULKAN_RENDER_PASS
#include "ColaStart.h"
#include <vulkan/vulkan.h>

namespace Cola
{
    class VulkanRenderPass
    {
    
    public:
        VulkanRenderPass(VkDevice *device, VkFormat swapChainImageFormat);
        ~VulkanRenderPass();

        const VkRenderPass &getRenderPass();
    private:
        void init();
        VkDevice *mDevice;
        VkFormat mSwapChainImageFormat; //交换链图像格式
        VkRenderPass mRenderPass = VK_NULL_HANDLE; //渲染过程
        std::vector<VkAttachmentDescription> colorAttachmentVec;
    };
    
    
    
}
#endif