#ifndef VULKAN_FRAME_BUFFER_H
#define VULKAN_FRAME_BUFFER_H
#include "ColaStart.h"
#include <vulkan/vulkan.h>

namespace Cola
{
    class VulkanFrameBuffer
    {
    public:
        VulkanFrameBuffer(VkDevice *dvice, const std::vector<VkImageView> &swapChainImageViewVec, const VkRenderPass &renderPass, uint32_t width, uint32_t height);
        ~VulkanFrameBuffer();

        VkFramebuffer getFramebuffer(size_t imageIndex); //获取帧缓冲
    
    private:
        VkDevice *mDevice;
        std::vector<VkFramebuffer> mFrameBufferVec;
    };
}

#endif