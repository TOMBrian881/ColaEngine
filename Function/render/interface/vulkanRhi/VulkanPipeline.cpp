#include "VulkanPipeline.h"

namespace Cola
{
    VulkanPipeline::VulkanPipeline(VkDevice *dvice, std::vector<std::shared_ptr<Cola::VulkanShader>> &shaderVec)
    :mDevice(mDevice)
    {
        mShaderVec = shaderVec;
        init();
    }
    
    VulkanPipeline::~VulkanPipeline()
    {
    }

    void VulkanPipeline::init()
    {
       std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfoVec;

       for (size_t i = 0; i < mShaderVec.size(); ++i)
       {
            std::shared_ptr<Cola::VulkanShader> shader = mShaderVec[i];
            VkPipelineShaderStageCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            createInfo.stage = shader->getStageFlagBits();
            createInfo.module = shader->getShaderModule();
            createInfo.pName = "main";

            shaderStageCreateInfoVec.push_back(createInfo);
       }


    }
}