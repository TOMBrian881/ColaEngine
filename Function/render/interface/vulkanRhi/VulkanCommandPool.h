#ifndef VULKAN_COMMAND_POOL
#define VULKAN_COMMAND_POOL
#include "ColaStart.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanPipeline.h"
#include <vulkan/vulkan.h>

namespace Cola
{
    class VulkanCommandPool
    {
    
    public:
        VulkanCommandPool(VkDevice *device, uint32_t graphicsFamilyIndex);
        ~VulkanCommandPool();

        void begin(); //开始记录命令缓冲区
        void cmdBeginRenderPass(VulkanRenderPass * renderPass, VulkanFrameBuffer* frameBuffer, size_t imageIndex , uint32_t width, uint32_t height); //开始渲染过程
        void cmdBindPipeline(VulkanPipeline* pipeline); //绑定管线
        void cmdSetViewport(float x, float y, float width, float height, float minDepth = 0.0, float maxDepth = 1.0); //设置视口
        void cmdSetScissor(float offsetX, float offsetY, float width, float height); //设置裁剪面
        void cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance); //绘制命令
        void end(); //结束

        const VkCommandBuffer &getCommandBuffer(){return mCommandBuffer;}
    private:
        VkDevice *mDevice;
        VkCommandPool mCommandPool; //命令池
        VkCommandBuffer mCommandBuffer; //命令缓冲池
    };
    
}


#endif