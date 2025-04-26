#include "VulkanRHI.h"
#include "log/LogSystem.h"
#include "Window/ColaGlfwWindow.h"
#include "Utilities.h"
#include "VulkanTool.h"

namespace Cola
{
    VulkanRHI::VulkanRHI(ColaWindow *window)
    {
        mWindow = window->getWindow();
        mSurface = VK_NULL_HANDLE;
        mRequiredExtensionsVec = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        Init();
    }

    VulkanRHI::~VulkanRHI()
    {
        if (mInstance != nullptr)
        {
            for (auto imageView : mSwapChainImageViewVec)
            {
                vkDestroyImageView(mDevice, imageView, nullptr);
            }

            vkDestroySwapchainKHR(mDevice, mSwapChain, nullptr);
            vkDestroyDevice(mDevice, nullptr);
            vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
            vkDestroyInstance(mInstance, nullptr);
            mInstance = nullptr;
        }
    }

    void VulkanRHI::Init()
    {
#ifndef NDEBUG
        mEnableValidationLayers = true;
        mEnableDebugUtilsLabel = true;
        AddExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#else
        mEnableValidationLayers = false;
        mEnableDebugUtilsLabel = false;
#endif

        if (mEnableValidationLayers)
        {
            AddLayer("VK_LAYER_KHRONOS_validation");
            // AddLayer("VK_LAYER_LUNARG_api_dump");
        }

        CreateInstance();

        createDebugMessenger();

        CreateWindowSurface();

        SelectPhysicalDevices();

        createLogicalDevice();

        createSwapChain();

        createImageView();
    }

    void VulkanRHI::AddLayerOrExtension(std::vector<const char*> &container, const char* name)
    {
        bool isFound = false;
        for (auto& i : container)
        {
            if (!strcmp(i, name))
            {
                isFound = true;
                break;
            }
        }

        if (!isFound)
        {
            LOG_DEBUG("container push back {0}", name);
            container.push_back(name);
        }
    }

    void VulkanRHI::AddLayer(const char* layerName)
    {
        AddLayerOrExtension(mLayerVec, layerName);
    }

    void VulkanRHI::AddExtension(const char* extensionName)
    {
        AddLayerOrExtension(mExtentionVec, extensionName);
    }

    void VulkanRHI::CreateInstance()
    {
        if (mEnableValidationLayers && !(CheckValidationLayerSupport()))
        {
            LOG_ERROR("validation layers requested, but not available!");
        }

        // 获取扩展
        uint32_t extensionCount = 0;
        const char** extensionNames;
        extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);
        if (!extensionNames)
        {
            LOG_ERROR("glfwGetRequiredInstanceExtensions failed!");
            return;
        }

        for (size_t i = 0; i < extensionCount; i++)
        {
            AddExtension(extensionNames[i]);
        }

        //获取Vulkan最新版本
        if (vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion"));
        {
            vkEnumerateInstanceVersion(&mVulkanApiVersion);
            LOG_INFO("Vulkan Api Version is {0}.{1}.{2}", VK_VERSION_MAJOR(mVulkanApiVersion) , VK_VERSION_MINOR(mVulkanApiVersion), VK_VERSION_PATCH(mVulkanApiVersion));
        }

        VkApplicationInfo appInfo {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
        appInfo.pApplicationName = "ColaRenderEngine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "ColaEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = mVulkanApiVersion;

        VkInstanceCreateInfo instanceCreateInfo {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mLayerVec.size());
        instanceCreateInfo.ppEnabledLayerNames = mLayerVec.data();
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(mExtentionVec.size());
        instanceCreateInfo.ppEnabledExtensionNames = mExtentionVec.data();

        if (vkCreateInstance(& instanceCreateInfo, nullptr, &mInstance) != VK_SUCCESS)
        {
            LOG_ERROR("Failed to create a vulkan instance");
        }

    }

    VkResult VulkanRHI::createDebugUtilsMessengerExt(VkInstance instance, 
                                                        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, 
                                                        const VkAllocationCallbacks * pAllocator, 
                                                        VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }


    // debug callback
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT,
                                                        VkDebugUtilsMessageTypeFlagsEXT,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void*)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    //创建debug消息层
    void VulkanRHI::createDebugMessenger()
    {
        if (mEnableValidationLayers)
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
            createInfo.pUserData = nullptr;
            createInfo.pNext = nullptr;
            createInfo.flags = 0;

            if (createDebugUtilsMessengerExt(mInstance, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS)
            {
                LOG_ERROR("failed to create debug messenger!");
            }
        }

        // if (mEnableDebugUtilsLabel)
        // {

        // }
    }

    bool VulkanRHI::CheckValidationLayerSupport()
    {
        bool result = true;
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char * layerName : mLayerVec)
        {
            bool bFound = false;
            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {
                LOG_ERROR("{0} not found in availableLayers", layerName);
                result = false;
                break;
            }
        }

        return result;
    }

    void VulkanRHI::CreateWindowSurface()
    {
        VkResult result = glfwCreateWindowSurface(mInstance, mWindow, nullptr, &mSurface);

        if (result != VK_SUCCESS)
        {
            LOG_ERROR("glfwCreateWindowSurface failed");
        }
        
    }

    void VulkanRHI::SelectPhysicalDevices()
    {
        uint32_t deviceCount = 0;
        VkResult result = vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);
        if (result != VK_SUCCESS || deviceCount == 0)
        {
            LOG_ERROR("Failed to find any physical device supports vulkan");
        }
        else
        {
            std::vector<VkPhysicalDevice> physicalDevicesVec(deviceCount);
            vkEnumeratePhysicalDevices(mInstance, &deviceCount, physicalDevicesVec.data());

            std::map<size_t, VkPhysicalDevice> physicalDevicesMap;

            for (size_t i = 0; i < deviceCount; i++)
            {
                VkPhysicalDeviceProperties props;
                vkGetPhysicalDeviceProperties(physicalDevicesVec[i], &props);

                size_t score = 0;

                if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                {
                    LOG_INFO("physicalDevices {0}: {1} (type == Discrete GPU)", i, props.deviceName);
                    score += 40;
                }
                else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
                {
                    LOG_INFO("physicalDevices {0}: {1} (type == Integrater GPU)", i, props.deviceName);
                    score += 30;
                }
                else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
                {
                    LOG_INFO("physicalDevices {0}: {1} (type == Virtual GPU)", i, props.deviceName);
                    score += 20;
                }
                else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
                {
                    LOG_INFO("physicalDevices {0}: {1} (type == CPU)", i, props.deviceName);
                    score += 10;
                }
                else
                {
                    LOG_INFO("physicalDevices {0}: {1} (type == Other)", i, props.deviceName);
                }

                physicalDevicesMap.insert(std::make_pair(score, physicalDevicesVec[i]));
            }

            for (std::map<size_t, VkPhysicalDevice>::reverse_iterator physicalDevicesMapIt = physicalDevicesMap.rbegin(); 
                    physicalDevicesMapIt != physicalDevicesMap.rend(); physicalDevicesMapIt++)
            {
                if (isDeviceSuitable(physicalDevicesMapIt->second))
                {
                    mPhysicalDevice = physicalDevicesMapIt->second;
                }
            }
        }
    }

    //判断是否是合适的物理设备
    bool VulkanRHI::isDeviceSuitable(const VkPhysicalDevice &physicalDevice)
    {
        bool result = false;

        QueueFamilyInfo queueFamilyInfo = getQueueFamilyInfo(physicalDevice);
        
        bool isEnableQueueFamily = false;

        //判断队列族是否同时满足图形、计算、窗口表面显示要求
        if (queueFamilyInfo.mGraphicIndex > -1 && queueFamilyInfo.mComputerIndex > -1 && queueFamilyInfo.mPresentIndex > -1)
        {
            isEnableQueueFamily = true;
            mQueueFamilyInfo = queueFamilyInfo;
        }

        bool isSupportExtension = CheckDeviceExtensinonSupport(physicalDevice); //是否支持扩展

        bool isSupportSwapChain = false;
        if (isSupportExtension)
        {
            SwapChainSupportDetail detail = querySwapChainSupport(physicalDevice);
            if (!detail.formatVec.empty() && !detail.presentModeVec.empty())
            {
                isSupportSwapChain = true;
            }
        }

        if (isEnableQueueFamily && isSupportSwapChain)
        {
            result = true;
        }

        return result;
    }

    bool VulkanRHI::CheckDeviceExtensinonSupport(VkPhysicalDevice physicalDevice)
    {
        bool result = false;
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        if (extensionCount > 0)
        {
            std::vector<VkExtensionProperties> extensionPropertiesVec(extensionCount);
            vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensionPropertiesVec.data());

            std::set<std::string> requireExtensionSet(mRequiredExtensionsVec.begin(), mRequiredExtensionsVec.end());

            for (auto extensionProperties : extensionPropertiesVec)
            {
                requireExtensionSet.erase(extensionProperties.extensionName);
            }

            if (requireExtensionSet.empty())
            {
                result = true;
            }
        }

        return result;
    }

    //获取物理设备内的队列族信息
    QueueFamilyInfo VulkanRHI::getQueueFamilyInfo(const VkPhysicalDevice &physicalDevice)
    {
        QueueFamilyInfo queueFamilyInfo;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        if (queueFamilyCount == 0)
        {
            LOG_ERROR("Failed to find any queue family properties");
        }
        else 
        {
            std::vector<VkQueueFamilyProperties> queueFamily(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamily.data());

            for (size_t i = 0; i < queueFamilyCount; ++i)
            {
                const VkQueueFlags &queueFlags = queueFamily[i].queueFlags;
                //判断是否满足图形渲染
                if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    queueFamilyInfo.mGraphicIndex = i;
                }

                //判断是否满足计算
                if (queueFlags & VK_QUEUE_COMPUTE_BIT)
                {
                    queueFamilyInfo.mComputerIndex = i;
                }

                //判断是否符合窗口显示
                VkBool32 isSupportPresent = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, mSurface, &isSupportPresent);

                if (isSupportPresent)
                {
                    queueFamilyInfo.mPresentIndex = i;
                }

                LOG_INFO("queueFamily {0} support:", i);
                LOG_INFO("    Graphics: {0}", queueFlags & VK_QUEUE_GRAPHICS_BIT);
                LOG_INFO("    Computer: {0}", queueFlags & VK_QUEUE_COMPUTE_BIT);
                LOG_INFO("    Present:  {0}", isSupportPresent);

                //都满足的情况就可以退出循环
                if ((queueFamilyInfo.mGraphicIndex > -1) && (queueFamilyInfo.mComputerIndex > -1) && (queueFamilyInfo.mPresentIndex > -1))
                {
                    LOG_INFO("queueFamily {0} enable all require", i);
                    break;
                }
            }
        }
        return queueFamilyInfo;
    }

    //创建逻辑设备
    void VulkanRHI::createLogicalDevice()
    {
        QueueFamilyInfo queueFamilyInfo = getQueueFamilyInfo(mPhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfoVec;

        //队列优先级
        float queuePriority = 1.0;

        std::set<int> uniqueQueueFamilys = {queueFamilyInfo.mGraphicIndex, queueFamilyInfo.mPresentIndex, queueFamilyInfo.mComputerIndex};

        for (int queueFamily : uniqueQueueFamilys)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.flags = 0;
            queueCreateInfo.pNext = nullptr;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfoVec.push_back(queueCreateInfo);
        }

        //指定需要支持的设备特性
        VkPhysicalDeviceFeatures deviceFeatures{};

        //创建逻辑设备
        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfoVec.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoVec.size());
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

        if (mEnableValidationLayers)
        {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mLayerVec.size());
            deviceCreateInfo.ppEnabledLayerNames = mLayerVec.data();
        }
        else
        {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(mRequiredExtensionsVec.size());
        deviceCreateInfo.ppEnabledExtensionNames = mRequiredExtensionsVec.data();

        if (vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create logical device!");
        }

        //获取图形队列
        vkGetDeviceQueue(mDevice, queueFamilyInfo.mGraphicIndex, 0, &mGraphicQueue);

        //获取显示队列
        vkGetDeviceQueue(mDevice, queueFamilyInfo.mGraphicIndex, 0, &mPresentQueue);
    }

    //查询交换链支持
    SwapChainSupportDetail VulkanRHI::querySwapChainSupport(const VkPhysicalDevice &device)
    {
        SwapChainSupportDetail swapChainDetail;

        //查询基本表面功能
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &swapChainDetail.capabilities);

        //查询支持的表面格式
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            swapChainDetail.formatVec.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, swapChainDetail.formatVec.data());
        }

        //查询支持的呈现模式
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            swapChainDetail.presentModeVec.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, swapChainDetail.presentModeVec.data());
        }

        return swapChainDetail;
    }

    //选择最佳的表面格式
    VkSurfaceFormatKHR VulkanRHI::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormatVec)
    {
        VkSurfaceFormatKHR availableSurfaceFormat = surfaceFormatVec[0];
        for (const auto &surfaceFormat : surfaceFormatVec)
        {
            if (surfaceFormat.format == VK_FORMAT_B8G8R8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                availableSurfaceFormat = surfaceFormat;
            }
        }

        return availableSurfaceFormat;
    }

    //选择最佳的显示模式
    VkPresentModeKHR VulkanRHI::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModeVec)
    {
        VkPresentModeKHR availablePresentMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto &presentMode : presentModeVec)
        {
            if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                availablePresentMode = presentMode;
            }
        }

        return availablePresentMode;
    }

    //选择最佳的交换范围（交换图像的分辨率）
    VkExtent2D VulkanRHI::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        VkExtent2D availableExtent;

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            availableExtent = capabilities.currentExtent;
        }
        else
        {
            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(mWindow, &width, &height);

            availableExtent.width = static_cast<uint32_t>(width);
            availableExtent.height = static_cast<uint32_t>(height);

            availableExtent.width = std::clamp(availableExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            availableExtent.height = std::clamp(availableExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        }

        return availableExtent;
    }

    //创建交换链
    void VulkanRHI::createSwapChain()
    {
        SwapChainSupportDetail swapChainSupport = querySwapChainSupport(mPhysicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formatVec);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModeVec);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        //指定交换链中需要包含多少图像
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if ((swapChainSupport.capabilities.maxImageCount > 0) && (imageCount > swapChainSupport.capabilities.maxImageCount))
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapChainCreateInfo{};
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainCreateInfo.surface = mSurface;
        swapChainCreateInfo.minImageCount = imageCount;
        swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapChainCreateInfo.imageFormat = surfaceFormat.format;
        swapChainCreateInfo.presentMode = presentMode;
        swapChainCreateInfo.imageExtent = extent;
        swapChainCreateInfo.imageArrayLayers = 1;
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyInfo queueFamilyInfo = getQueueFamilyInfo(mPhysicalDevice);
        uint32_t queueFamilyIndices[]  = {queueFamilyInfo.mGraphicIndex, queueFamilyInfo.mPresentIndex};

        if (queueFamilyInfo.mGraphicIndex != queueFamilyInfo.mPresentIndex)
        {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapChainCreateInfo.queueFamilyIndexCount = 2;
            swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapChainCreateInfo.queueFamilyIndexCount = 0;
            swapChainCreateInfo.pQueueFamilyIndices = nullptr;
        }

        swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainCreateInfo.clipped = VK_TRUE;
        swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
        
        if (vkCreateSwapchainKHR(mDevice, &swapChainCreateInfo, nullptr, &mSwapChain) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create swap chain");
        }

        vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, nullptr);
        mSwapChainImageVec.resize(imageCount);
        vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, mSwapChainImageVec.data());

        mSwapChainImageFormat = surfaceFormat.format;
        mSwapChainExtent = extent;
    }

    //创建图像视图
    void VulkanRHI::createImageView()
    {
        mSwapChainImageViewVec.resize(mSwapChainImageVec.size());

        for (size_t i = 0; i < mSwapChainImageVec.size(); ++i)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = mSwapChainImageVec[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = mSwapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(mDevice, &createInfo, nullptr, &mSwapChainImageViewVec[i]) != VK_SUCCESS)
            {
                LOG_ERROR("failed to create image views!");
            }
        }
    }

    //获取逻辑设备
    VkDevice *VulkanRHI::getDevice()
    {
        return &mDevice;
    }

    //获取交换链图像格式
    VkFormat VulkanRHI::getFormat()
    {
        return mSwapChainImageFormat;
    }

    //获取交换链图像视图数组
    const std::vector<VkImageView> &VulkanRHI::getSwapChainImageViewVec()
    {
        return mSwapChainImageViewVec;
    }

    //获取交换链交换范围
    const VkExtent2D &VulkanRHI::getSwapChainExtent()
    {
        return mSwapChainExtent;
    }

    //获取交换链
    const VkSwapchainKHR &VulkanRHI::getSwapChain()
    {
        return mSwapChain;
    }

    //获取渲染队列族下标
    uint32_t VulkanRHI::getGrapicksFamilyIndex()
    {
        return static_cast<uint32_t>(mQueueFamilyInfo.mGraphicIndex);
    }
}