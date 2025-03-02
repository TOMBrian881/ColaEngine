#include "log/LogSystem.h"
#include "ColaWindow.h"
#include "render/interface/GraphicRhi.h"
#include "render/interface/vulkanRhi/VulkanPipeline.h"
#include "render/interface/vulkanRhi/VulkanRHI.h"
#include "render/interface/vulkanRhi/VulkanShader.h"


int main()
{
    std::unique_ptr<Cola::ColaWindow> window = Cola::ColaWindow::Create(800, 600, "SanBox");

    std::unique_ptr<Cola::GraphicRhi> graphicRhi = Cola::GraphicRhi::Create(window.get());

    Cola::VulkanRHI *vulkanRhi = dynamic_cast<Cola::VulkanRHI *>(graphicRhi.get());

    VkDevice *device = vulkanRhi->getDevice();

    std::vector<std::shared_ptr<Cola::VulkanShader>> shaderVec;

    std::string triangleVsFilePath = COLA_RES_SPIR_V_DIR"triangleVs.spv";
    std::string triangleFsFilePath = COLA_RES_SPIR_V_DIR"triangleFs.spv";
    std::shared_ptr<Cola::VulkanShader> vertexShader = std::make_shared<Cola::VulkanShader>(device, triangleVsFilePath, Cola::VulkanShader::ShaderType::VERTEX);
    std::shared_ptr<Cola::VulkanShader> fragShader = std::make_shared<Cola::VulkanShader>(device, triangleFsFilePath, Cola::VulkanShader::ShaderType::FRAGMENT);

    shaderVec.push_back(vertexShader);
    shaderVec.push_back(fragShader);

    std::shared_ptr<Cola::VulkanPipeline> pipeline = std::make_shared<Cola::VulkanPipeline>(device, shaderVec);

    while (!window->ShouldClose())
    {
        window->PollEvents();


        window->SwapBuffers();
    }
}