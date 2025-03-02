
#ifndef VULKAN_RHI_H
#define VULKAN_RHI_H
#include "../GraphicRhi.h"
#include "VulkanCommon.h"
#include <vulkan/vulkan.h>


namespace Cola
{
    class VulkanRHI : public GraphicRhi
    {
    public:
        VulkanRHI(ColaWindow *window);
        ~VulkanRHI() override;

        VkDevice *getDevice(); //获取逻辑设备

    private:
        void Init();
        void AddLayerOrExtension(std::vector<const char*> &container, const char* name);
        void AddLayer(const char* layerName);
        void AddExtension(const char* extensionName);
        void CreateInstance(); //创建vulkan实例
        void CreateWindowSurface(); //创建Window Surface;
        void SelectPhysicalDevices(); //选择物理设备
        bool isDeviceSuitable(const VkPhysicalDevice &physicalDevice); //判断是否是合适的物理设备
        bool CheckValidationLayerSupport(); //检查验证层是否支持
        bool CheckDeviceExtensinonSupport(VkPhysicalDevice physicalDevice); //检查设备扩展是否支持
        QueueFamilyInfo getQueueFamilyInfo(const VkPhysicalDevice &physicalDevice); //获取物理设备内的队列族信息
        void createLogicalDevice(); //创建逻辑设备
        SwapChainSupportDetail querySwapChainSupport(const VkPhysicalDevice &device); //查询交换链支持
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormatVec); //选择最佳的表面格式
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModeVec); //选择最佳的显示模式
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities); //选择最佳的交换范围（交换图像的分辨率）
        void createSwapChain(); //创建交换链
        void createImageView(); //创建图像视图
        VkInstance mInstance;
        std::vector<const char*> mLayerVec; //验证层数组
        std::vector<const char*> mExtentionVec; //扩展层数组
        GLFWwindow *mWindow;
        bool mEnableValidationLayers; //是否开启检查验证层支持
        bool mEnableDebugUtilsLabel; //是否启用调试实用程序标签
        uint32_t mVulkanApiVersion = VK_API_VERSION_1_0; //Vulkan api的版本
        VkSurfaceKHR mSurface; //平面KHR
        VkPhysicalDevice mPhysicalDevice; //物理设备
        QueueFamilyInfo mQueueFamilyInfo; //队列族信息
        std::vector<char const*> mRequiredExtensionsVec; //需要的扩展数组
        VkDevice mDevice; //逻辑设备
        VkQueue mGraphicQueue; //图形队列
        VkQueue mPresentQueue; //显示队列
        VkSwapchainKHR mSwapChain; //交换链
        std::vector<VkImage> mSwapChainImageVec; //交换链图像数组
        VkFormat mSwapChainImageFormat; //交换链图像格式
        VkExtent2D mSwapChainExtent; //交换链交换范围（交换图像的分辨率）
        std::vector<VkImageView> mSwapChainImageViewVec; //交换链图像视图
    };
}


#endif