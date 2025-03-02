#include "VulkanTool.h"
#include "Utilities.h"
#include "log/LogSystem.h"

namespace Cola
{
    namespace VulkanTool
    {
        VkShaderModule loadShader(const std::string filePath, const VkDevice &device)
        {
            std::vector<char> shaderCode = Utilities::readFile(filePath);

            VkShaderModuleCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize = shaderCode.size();
            createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

            VkShaderModule shaderModule;
            if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            {
                LOG_ERROR("failed to create shader module, shader file: {0}", filePath);
            }

            return shaderModule;
        }
    }
}