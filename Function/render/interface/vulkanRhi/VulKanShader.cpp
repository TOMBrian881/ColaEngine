#include "VulkanShader.h"
#include "VulkanTool.h"

namespace Cola
{
    VulkanShader::VulkanShader(VkDevice *device, const std::string &filePath, ShaderType type)
    :mDevice(device),
    mType(type)
    {
        mShaderModule = VulkanTool::loadShader(filePath, *mDevice);
    }
        
    VulkanShader::~VulkanShader()
    {
        vkDestroyShaderModule(*mDevice, mShaderModule, nullptr);
    }

    VkShaderModule VulkanShader::getShaderModule()
    {
        return mShaderModule;
    }

    VkShaderStageFlagBits VulkanShader::getStageFlagBits()
    {
        VkShaderStageFlagBits stageFlagBit = VkShaderStageFlagBits::VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
        switch (mType)
        {
        case ShaderType::VERTEX:
            stageFlagBit = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case ShaderType::FRAGMENT:
            stageFlagBit = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        default:
            break;
        }

        return stageFlagBit;
    }
}
