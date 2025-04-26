#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H
#include "ColaStart.h"
#include "VulkanShader.h"
#include <vulkan/vulkan.h>
#include "VulkanRenderPass.h"

namespace Cola
{
    class VulkanPipeline
    {
    public:
        VulkanPipeline(VkDevice *dvice, std::vector<std::shared_ptr<Cola::VulkanShader>> &shaderVec, VulkanRenderPass *renderPass);
        ~VulkanPipeline();

        void init();

        const VkPipeline &getPipeline(); //获取管线

    private:
        VkDevice *mDevice;
        std::vector<std::shared_ptr<Cola::VulkanShader>> mShaderVec;
        VkPipelineLayout mPipelineLayout; //管道布局
        VulkanRenderPass *mRenderPass; //渲染过程
        VkPipeline mGraphicPipeline; //渲染管道
    };

}
#endif