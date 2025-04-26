#include "VulkanPipeline.h"
#include "log/LogSystem.h"

namespace Cola
{
    VulkanPipeline::VulkanPipeline(VkDevice *dvice, std::vector<std::shared_ptr<Cola::VulkanShader>> &shaderVec, VulkanRenderPass *renderPass)
    :mDevice(dvice),
    mShaderVec(shaderVec),
    mRenderPass(renderPass)
    {
        init();
    }
    
    VulkanPipeline::~VulkanPipeline()
    {
        vkDestroyPipeline(*mDevice, mGraphicPipeline, nullptr);
        vkDestroyPipelineLayout(*mDevice, mPipelineLayout, nullptr);
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

       //顶点输入
       VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
       vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
       vertexInputInfo.vertexBindingDescriptionCount = 0;
       vertexInputInfo.pVertexBindingDescriptions = nullptr;
       vertexInputInfo.vertexAttributeDescriptionCount = 0;
       vertexInputInfo.pVertexAttributeDescriptions = nullptr;

       //指定顶点输入组件
       VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
       inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
       inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
       inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

       //视口
       VkViewport viewport{};
       viewport.x = 0.0;
       viewport.y = 0.0;
       viewport.width = 100.0;
       viewport.height = 100.0;
       viewport.minDepth = 0.0;
       viewport.maxDepth = 1.0;

       //裁剪
       VkRect2D scissor{};
       scissor.offset = {0, 0};
       scissor.extent = {100, 100};

       //动态状态集
       std::vector<VkDynamicState> dynamicStateVec = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
       };
       VkPipelineDynamicStateCreateInfo dynamicState{};
       dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
       dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateVec.size());
       dynamicState.pDynamicStates = dynamicStateVec.data();

       //创建视口
       VkPipelineViewportStateCreateInfo viewportState{};
       viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
       viewportState.viewportCount = 1;
       viewportState.pViewports = &viewport;
       viewportState.scissorCount = 1;
       viewportState.pScissors = &scissor;

       //光栅化
       VkPipelineRasterizationStateCreateInfo rasterizerState{};
       rasterizerState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
       rasterizerState.depthBiasEnable = VK_FALSE;
       rasterizerState.rasterizerDiscardEnable = VK_FALSE;
       rasterizerState.polygonMode = VK_POLYGON_MODE_FILL;
       rasterizerState.lineWidth = 1.0;
       rasterizerState.cullMode = VK_CULL_MODE_BACK_BIT;
       rasterizerState.frontFace = VK_FRONT_FACE_CLOCKWISE;
       rasterizerState.depthBiasEnable = VK_FALSE;
       rasterizerState.depthBiasConstantFactor = 0.0;
       rasterizerState.depthBiasClamp = 0.0;
       rasterizerState.depthBiasSlopeFactor = 0.0;

       //多重采样
       VkPipelineMultisampleStateCreateInfo multisampleState{};
       multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
       multisampleState.sampleShadingEnable = VK_FALSE;
       multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
       multisampleState.minSampleShading = 1.0;
       multisampleState.pSampleMask = nullptr;
       multisampleState.alphaToCoverageEnable = VK_FALSE;
       multisampleState.alphaToOneEnable = VK_FALSE;

       //深度与模板测试


       //颜色混合
       VkPipelineColorBlendAttachmentState colorBlendAttachment{};
       colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
       colorBlendAttachment.blendEnable = VK_FALSE;
       colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
       colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
       colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
       colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
       colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
       colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

       //透明混合模式需要设置的值
       VkPipelineColorBlendStateCreateInfo colorBlendState{};
       colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
       colorBlendState.logicOpEnable = VK_FALSE;
       colorBlendState.logicOp = VK_LOGIC_OP_COPY;
       colorBlendState.attachmentCount = 1;
       colorBlendState.pAttachments = &colorBlendAttachment;
       colorBlendState.blendConstants[0] = 0.0;
       colorBlendState.blendConstants[1] = 0.0;
       colorBlendState.blendConstants[2] = 0.0;
       colorBlendState.blendConstants[3] = 0.0;

       VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
       pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
       pipelineLayoutInfo.setLayoutCount = 0;
       pipelineLayoutInfo.pSetLayouts = nullptr;
       pipelineLayoutInfo.pushConstantRangeCount = 0;
       pipelineLayoutInfo.pPushConstantRanges = nullptr;

       if (vkCreatePipelineLayout(*mDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS)
       {
            LOG_ERROR("failed to create pipeline layout!");
       }

       //渲染管道
       VkGraphicsPipelineCreateInfo pipelineInfo{};
       pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
       pipelineInfo.stageCount = static_cast<uint32_t>(shaderStageCreateInfoVec.size());
       pipelineInfo.pStages = shaderStageCreateInfoVec.data();
       pipelineInfo.pVertexInputState = &vertexInputInfo;
       pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
       pipelineInfo.pViewportState = &viewportState;
       pipelineInfo.pRasterizationState = &rasterizerState;
       pipelineInfo.pMultisampleState = &multisampleState;
       pipelineInfo.pDepthStencilState = nullptr;
       pipelineInfo.pColorBlendState = &colorBlendState;
       pipelineInfo.pDynamicState = &dynamicState;
       pipelineInfo.layout = mPipelineLayout;
       pipelineInfo.renderPass = mRenderPass->getRenderPass();
       pipelineInfo.subpass = 0;
       pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
       pipelineInfo.basePipelineIndex = -1;

       if (vkCreateGraphicsPipelines(*mDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mGraphicPipeline) != VK_SUCCESS)
       {
            LOG_ERROR("failed to create graphic pipeline!");
       }
    }

    //获取管线
    const VkPipeline &VulkanPipeline::getPipeline()
    {
       return mGraphicPipeline;
    }
}