#include "log/LogSystem.h"
#include "ColaWindow.h"
#include "render/interface/GraphicRhi.h"
#include "render/interface/vulkanRhi/VulkanPipeline.h"
#include "render/interface/vulkanRhi/VulkanRHI.h"
#include "render/interface/vulkanRhi/VulkanShader.h"
#include "render/interface/vulkanRhi/VulkanRenderPass.h"
#include "render/interface/vulkanRhi/VulkanFrameBuffer.h"
#include "render/interface/vulkanRhi/VulkanCommandPool.h"



int main()
{
    std::unique_ptr<Cola::ColaWindow> window = Cola::ColaWindow::Create(800, 600, "SanBox");

    std::unique_ptr<Cola::GraphicRhi> graphicRhi = Cola::GraphicRhi::Create(window.get());

    Cola::VulkanRHI *vulkanRhi = dynamic_cast<Cola::VulkanRHI *>(graphicRhi.get());

    VkDevice *device = vulkanRhi->getDevice();

    VkFormat swapChainFormat = vulkanRhi->getFormat();
    
    std::shared_ptr<Cola::VulkanRenderPass> renderPass = std::make_shared<Cola::VulkanRenderPass>(device, swapChainFormat);

    std::vector<std::shared_ptr<Cola::VulkanShader>> shaderVec;

    std::string triangleVsFilePath = COLA_RES_SPIR_V_DIR"triangleVs.spv";
    std::string triangleFsFilePath = COLA_RES_SPIR_V_DIR"triangleFs.spv";
    std::shared_ptr<Cola::VulkanShader> vertexShader = std::make_shared<Cola::VulkanShader>(device, triangleVsFilePath, Cola::VulkanShader::ShaderType::VERTEX);
    std::shared_ptr<Cola::VulkanShader> fragShader = std::make_shared<Cola::VulkanShader>(device, triangleFsFilePath, Cola::VulkanShader::ShaderType::FRAGMENT);

    shaderVec.push_back(vertexShader);
    shaderVec.push_back(fragShader);

    std::shared_ptr<Cola::VulkanPipeline> pipeline = std::make_shared<Cola::VulkanPipeline>(device, shaderVec, renderPass.get());

    const std::vector<VkImageView> swapChainImageViewVec = vulkanRhi->getSwapChainImageViewVec();
    const VkExtent2D swapChainExtend2D = vulkanRhi->getSwapChainExtent();

    std::shared_ptr<Cola::VulkanFrameBuffer> frameBuffer = std::make_shared<Cola::VulkanFrameBuffer>(device, swapChainImageViewVec, renderPass->getRenderPass(), swapChainExtend2D.width, swapChainExtend2D.height);

    uint32_t graphicIndex = vulkanRhi->getGrapicksFamilyIndex();
    std::shared_ptr<Cola::VulkanCommandPool> commandPool = std::make_shared<Cola::VulkanCommandPool>(device, graphicIndex);

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


    if (vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS)
    {
        LOG_ERROR("failed to create image Available Semaphore!");
    }

    if (vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
    {
        LOG_ERROR("failed to create render finished Semaphore!");
    }

    if (vkCreateFence(*device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        LOG_ERROR("failed to create fence!");
    }



    const VkSwapchainKHR &swapChain = vulkanRhi->getSwapChain();
    const VkQueue &graphicQueue = vulkanRhi->getGraphicQueue();
    const VkQueue &presentQueue = vulkanRhi->getPresentQueue();

    int count = 0;

    while (!window->ShouldClose())
    {
        // std::cout << "frame count = " << count << std::endl;
        // count++;
        window->PollEvents();

        vkWaitForFences(*device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(*device, 1, &inFlightFence);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(*device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        commandPool->begin();
        commandPool->cmdBeginRenderPass(renderPass.get(), frameBuffer.get(), imageIndex, swapChainExtend2D.width, swapChainExtend2D.height);

        commandPool->cmdBindPipeline(pipeline.get());
        commandPool->cmdSetViewport(0.0, 0.0, swapChainExtend2D.width, swapChainExtend2D.height);
        commandPool->cmdSetScissor(0.0, 0.0, swapChainExtend2D.width, swapChainExtend2D.height);
        commandPool->cmdDraw(3, 1, 0, 0);
        commandPool->end();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandPool->getCommandBuffer();

        VkSemaphore signalSemphores[] = {renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemphores;

        if (vkQueueSubmit(graphicQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
        {
            LOG_ERROR("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        vkQueuePresentKHR(presentQueue, &presentInfo);

        

        window->SwapBuffers();
    }

    vkDestroySemaphore(*device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(*device, renderFinishedSemaphore, nullptr);

}