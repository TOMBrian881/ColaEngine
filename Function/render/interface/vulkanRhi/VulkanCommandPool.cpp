#include "VulkanCommandPool.h"
#include "log/LogSystem.h"

namespace Cola
{
    VulkanCommandPool::VulkanCommandPool(VkDevice *device, uint32_t graphicsFamilyIndex)
    :mDevice(device)
    {
        VkCommandPoolCreateInfo commandPoolInfo{};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolInfo.queueFamilyIndex = graphicsFamilyIndex;

        if(vkCreateCommandPool(*mDevice, &commandPoolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create command pool!");
        }

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = mCommandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(*device, &allocateInfo, &mCommandBuffer) != VK_SUCCESS)
        {
            LOG_ERROR("failed to allocate command buffer!");
        }
    }
    
    VulkanCommandPool::~VulkanCommandPool()
    {
        vkDestroyCommandPool(*mDevice, mCommandPool, nullptr);
    }

    //开始记录命令缓冲区
    void VulkanCommandPool::begin()
    {
        vkResetCommandBuffer(mCommandBuffer, 0);
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(mCommandBuffer, &beginInfo) != VK_SUCCESS)
        {
            LOG_ERROR("failed to begin command buffer!");
        }
    }

    //开始渲染过程
    void VulkanCommandPool::cmdBeginRenderPass(VulkanRenderPass* renderPass, VulkanFrameBuffer* frameBuffer, size_t imageIndex , uint32_t width, uint32_t height)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->getRenderPass();
        renderPassInfo.framebuffer = frameBuffer->getFramebuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent.width = width;
        renderPassInfo.renderArea.extent.height = height;

        VkClearValue clearColor = {0.0, 0.0, 0.0, 1.0};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(mCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    //绑定管线
    void VulkanCommandPool::cmdBindPipeline(VulkanPipeline* pipeline)
    {
        vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());
    }

    //设置视口
    void VulkanCommandPool::cmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
    {
        VkViewport viewport{};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = minDepth;
        viewport.maxDepth = maxDepth;

        vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);
    }

    //设置裁剪面
    void VulkanCommandPool::cmdSetScissor(float offsetX, float offsetY, float width, float height)
    {
        VkRect2D scissor{};
        scissor.offset.x = offsetX;
        scissor.offset.y = offsetY;
        scissor.extent.width = width;
        scissor.extent.height = height;

        vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);
    }

    //绘制命令
    void VulkanCommandPool::cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
        vkCmdDraw(mCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    }

    //结束
    void VulkanCommandPool::end()
    {
        //结束渲染过程
        vkCmdEndRenderPass(mCommandBuffer);

        if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS)
        {
            LOG_ERROR("failed to end command buffer!");
        }
    }

}