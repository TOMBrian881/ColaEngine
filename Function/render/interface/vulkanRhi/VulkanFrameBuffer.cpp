#include "VulkanFrameBuffer.h"
#include "log/LogSystem.h"

namespace Cola
{
    VulkanFrameBuffer::VulkanFrameBuffer(VkDevice *dvice, const std::vector<VkImageView> &swapChainImageViewVec, const VkRenderPass &renderPass, uint32_t width, uint32_t height)
    :mDevice(dvice)
    {

        if (renderPass == VK_NULL_HANDLE) {
            LOG_ERROR("RenderPass is invalid");
        }

        mFrameBufferVec.resize(swapChainImageViewVec.size());
        for (size_t i = 0; i < swapChainImageViewVec.size(); ++i)
        {
            VkImageView attachment[] = { swapChainImageViewVec[i] };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachment;
            framebufferInfo.width = width;
            framebufferInfo.height = height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(*mDevice, &framebufferInfo, nullptr, &mFrameBufferVec[i]) != VK_SUCCESS)
            {
                LOG_ERROR("failed to create frame buffer!");
            }
        }
    }
    
    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        for (auto framebuffer : mFrameBufferVec)
        {
            vkDestroyFramebuffer(*mDevice, framebuffer, nullptr);
        }
        
    }

    //获取帧缓冲
    VkFramebuffer VulkanFrameBuffer::getFramebuffer(size_t imageIndex)
    {
        VkFramebuffer framebuffer = VK_NULL_HANDLE;
        if (imageIndex < mFrameBufferVec.size())
        {
            framebuffer = mFrameBufferVec[imageIndex];
        }

        return framebuffer;
    }
}