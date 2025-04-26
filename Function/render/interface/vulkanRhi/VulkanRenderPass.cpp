#include "VulkanRenderPass.h"
#include "log/LogSystem.h"

namespace Cola
{
    VulkanRenderPass::VulkanRenderPass(VkDevice *device, VkFormat swapChainImageFormat)
    :mDevice(device),
    mSwapChainImageFormat(swapChainImageFormat)
    {
        init();
    }
    
    VulkanRenderPass::~VulkanRenderPass()
    {
        vkDestroyRenderPass(*mDevice, mRenderPass, nullptr);
        mRenderPass = VK_NULL_HANDLE;
    }

    void VulkanRenderPass::init()
    {
        size_t attachmentCount = 1;
        // std::vector<VkAttachmentDescription> colorAttachmentVec(attachmentCount);
        colorAttachmentVec.resize(attachmentCount);
        for (size_t i = 0; i < colorAttachmentVec.size(); ++i)
        {
            VkAttachmentDescription &colorAttachment = colorAttachmentVec[i];
            colorAttachment.format = mSwapChainImageFormat;
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }
        

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(colorAttachmentVec.size());
        renderPassInfo.pAttachments = colorAttachmentVec.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if(vkCreateRenderPass(*mDevice, &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create render pass!");
        }
    }

    const VkRenderPass &VulkanRenderPass::getRenderPass()
    {
        return mRenderPass;
    }
}