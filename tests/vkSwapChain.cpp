#ifdef IS_PLATFORM_WIN
#include <Windows.h>
#endif

#include "vkew.h"

#ifdef IS_PLATFORM_WIN
#include <vulkan/vulkan_win32.h>
#endif

#include "vkSwapChain.h"


/// @brief 
/// @param surface_capabilities 
/// @return 
uint32_t SwapChain::GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR* surface_capabilities)
{
    // Set of images defined in a swap chain may not always be available for application to render to:
    // One may be displayed and one may wait in a queue to be presented
    // If application wants to use more images at the same time it must ask for more images
    uint32_t image_count = surface_capabilities->minImageCount + 1;
    if ((surface_capabilities->maxImageCount > 0) &&
        (image_count > surface_capabilities->maxImageCount))
    {
        image_count = surface_capabilities->maxImageCount;
    }
    return image_count;
}
/// @brief 
/// @param surface_formats 
/// @param count 
/// @param colorFormat 
/// @param colorSpace 
/// @return 
VkSurfaceFormatKHR SwapChain::GetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count, VkFormat colorFormat,
    VkColorSpaceKHR colorSpace)
{
    // If the list contains only one entry with undefined format
    // it means that there are no preferred surface formats and any can be chosen
    int i;
    if ((count == 1) &&
        (surface_formats[0].format == VK_FORMAT_UNDEFINED))
    {
        VkSurfaceFormatKHR ret;
        ret.colorSpace = colorSpace;
        ret.format = colorFormat;
        return ret;
    }
    // Check if list contains most widely used R8 G8 B8 A8 format
    // with nonlinear color space
    for (i = 0; i < count; i++)
    {
        if (surface_formats[i].format == colorFormat &&
            surface_formats[i].colorSpace == colorSpace)
        {
            return surface_formats[i];
        }
    }
    // Return the first format from the list
    return surface_formats[0];
}
/// @brief 
/// @param surface_capabilities 
/// @return 
VkExtent2D SwapChain::GetSwapChainExtent(const VkSurfaceCapabilitiesKHR* surface_capabilities)
{
    // Special value of surface extent is width == height == -1
    // If this is so we define the size by ourselves but it must fit within defined confines
    if (surface_capabilities->currentExtent.width == -1)
    {
        VkExtent2D swap_chain_extent = { 640, 480 };
        if (swap_chain_extent.width < surface_capabilities->minImageExtent.width)
        {
            swap_chain_extent.width = surface_capabilities->minImageExtent.width;
        }
        if (swap_chain_extent.height < surface_capabilities->minImageExtent.height)
        {
            swap_chain_extent.height = surface_capabilities->minImageExtent.height;
        }
        if (swap_chain_extent.width > surface_capabilities->maxImageExtent.width)
        {
            swap_chain_extent.width = surface_capabilities->maxImageExtent.width;
        }
        if (swap_chain_extent.height > surface_capabilities->maxImageExtent.height)
        {
            swap_chain_extent.height = surface_capabilities->maxImageExtent.height;
        }
        return swap_chain_extent;
    }
    // Most of the cases we define size of the swap_chain images equal to current window's size
    return surface_capabilities->currentExtent;
}
/// @brief 
/// @param surface_capabilities 
/// @return 
VkImageUsageFlags SwapChain::GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR* surface_capabilities)
{
    // Color attachment flag must always be supported
    // We can define other usage flags but we always need to check if they are supported
    if (surface_capabilities->supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
    {
        return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    return -1;
}
/// @brief 
/// @param surface_capabilities 
/// @return 
VkSurfaceTransformFlagBitsKHR SwapChain::GetSwapChainTransform(const VkSurfaceCapabilitiesKHR* surface_capabilities)
{
    // Sometimes images must be transformed before they are presented (i.e. due to device's orienation
    // being other than default orientation)
    // If the specified transform is other than current transform, presentation engine will transform image
    // during presentation operation; this operation may hit performance on some platforms
    // Here we don't want any transformations to occur so if the identity transform is supported use it
    // otherwise just use the same transform as current transform
    if (surface_capabilities->supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    return surface_capabilities->currentTransform;
}
/// @brief 
/// @param present_modes 
/// @param count 
/// @param vsync 
/// @return 
VkPresentModeKHR SwapChain::GetSwapChainPresentMode(const VkPresentModeKHR* present_modes, int count, int vsync)
{
    // FIFO present mode is always available
    // MAILBOX is the lowest latency -Sync enabled mode (something like triple-buffering) so use it if available
    int i;
    // Not vsync => VK_PRESENT_MODE_IMMEDIATE_KHR
    if (vsync == 0)
    {
        // IMMEDIATE is allowed to swap any time (even during scan-out; so tearing may occur).
        for (i = 0; i < count; i++)
            if (VK_PRESENT_MODE_IMMEDIATE_KHR == present_modes[i])
                return present_modes[i];
        // MAILBOX is like FIFO but has only one item queue, and it alows to replace the item in the queue by subsequent vkPresent.
        for (i = 0; i < count; i++)
            if (VK_PRESENT_MODE_MAILBOX_KHR == present_modes[i])
                return present_modes[i];
        // FIFO_RELAXED is like FIFO except it behaves like IMMEDIATE in the case that it had no image in queue for the last VBLANK swap (i.e. prefers tearing rather than showing old image).
        for (i = 0; i < count; i++)
            if (VK_PRESENT_MODE_FIFO_RELAXED_KHR == present_modes[i])
                return present_modes[i];
    }
    // FIFO means first-in–first-out — it will queue images you present and swap them one by one in the order given, during the VBLANK interval.
    for (i = 0; i < count; i++)
        if (VK_PRESENT_MODE_FIFO_KHR == present_modes[i])
            return present_modes[i];
    return present_modes[0]; // We need something  ...
}

/// @brief 
/// @param platformWindow 
/// @param physicalDevice 
/// @param device 
/// @param presentationSurface 
/// @param desired_extent 
/// @param colorFormat 
/// @param colorSpace 
/// @param vsync 
/// @param full_screen_exclusive 
/// @param previous 
/// @return 
std::shared_ptr<SwapChain> SwapChain::Create(
    void* platformWindow,
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    SurfaceRef presentationSurface,
    VkExtent2D& desired_extent,
    VkFormat colorFormat,
    VkColorSpaceKHR colorSpace,
    bool vsync,
    bool full_screen_exclusive,
    std::shared_ptr<SwapChain> previous)
{
    auto ret = std::make_shared<SwapChain>();
    ret->Value = VK_NULL_HANDLE;

    VkSurfaceCapabilitiesKHR surface_capabilities;
    if (VKEW_KHR_swapchain == 0)
    {
        return nullptr;
    }
    if (VK_CHECK(
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, presentationSurface->Value, &
            surface_capabilities)) != VK_SUCCESS)
    {
        return nullptr;
    }
    uint32_t formats_count;
    if ((VK_CHECK(
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, presentationSurface->Value, &formats_count, NULL
        )) != VK_SUCCESS) ||
        (formats_count == 0))
    {
        return nullptr;
    }
    VkSurfaceFormatKHR* surface_formats = new VkSurfaceFormatKHR[formats_count];
    if (VK_CHECK(
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, presentationSurface->Value, &formats_count, &
            surface_formats[0])) != VK_SUCCESS)
    {
        return nullptr;
    }
    uint32_t present_modes_count;
    if ((VK_CHECK(
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, presentationSurface->Value, &
            present_modes_count, NULL)) != VK_SUCCESS) ||
        (present_modes_count == 0))
    {
        return nullptr;
    }
    VkPresentModeKHR* present_modes = new VkPresentModeKHR[present_modes_count];
    if (VK_CHECK(
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, presentationSurface->Value, &
            present_modes_count, &present_modes[0])) != VK_SUCCESS)
    {
        return nullptr;
    }
    uint32_t desired_number_of_images = GetSwapChainNumImages(&surface_capabilities);
    VkSurfaceFormatKHR desired_format = GetSwapChainFormat(surface_formats, formats_count, colorFormat, colorSpace);
    if (desired_extent.width == 0 && desired_extent.height == 0)
    {
        desired_extent = GetSwapChainExtent(&surface_capabilities);
    }
    VkImageUsageFlags desired_usage = GetSwapChainUsageFlags(&surface_capabilities);
    VkSurfaceTransformFlagBitsKHR desired_transform = GetSwapChainTransform(&surface_capabilities);
    VkPresentModeKHR desired_present_mode = GetSwapChainPresentMode(present_modes, present_modes_count, vsync);
    VkSwapchainKHR old_swap_chain = previous ? previous->Value : VK_NULL_HANDLE;
    if ((int)(desired_usage) == -1)
    {
        return nullptr;
    }
    if ((int)(desired_present_mode) == -1)
    {
        return nullptr;
    }
    if ((desired_extent.width == 0) || (desired_extent.height == 0))
    {
        // Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
        // But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
        return ret;
    }

    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (uint32_t i = 0; i < 4; i++)
    {
        if (surface_capabilities.supportedCompositeAlpha & compositeAlphaFlags[i])
        {
            compositeAlpha = compositeAlphaFlags[i];
            break;
        }
    }
    VkSwapchainCreateInfoKHR swap_chain_create_info = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, // VkStructureType                sType
        NULL, // const void                    *pNext
        0, // VkSwapchainCreateFlagsKHR      flags
        presentationSurface->Value, // VkSurfaceKHR                   surface
        desired_number_of_images, // uint32_t                       minImageCount
        desired_format.format, // VkFormat                       imageFormat
        desired_format.colorSpace, // VkColorSpaceKHR                imageColorSpace
        desired_extent, // VkExtent2D                     imageExtent
        1, // uint32_t                       imageArrayLayers
        desired_usage, // VkImageUsageFlags              imageUsage
        VK_SHARING_MODE_EXCLUSIVE, // VkSharingMode                  imageSharingMode
        0, // uint32_t                       queueFamilyIndexCount
        NULL, // const uint32_t                *pQueueFamilyIndices
        desired_transform, // VkSurfaceTransformFlagBitsKHR  preTransform
        compositeAlpha, // VkCompositeAlphaFlagBitsKHR    compositeAlpha
        desired_present_mode, // VkPresentModeKHR               presentMode
        VK_TRUE, // VkBool32                       clipped
        old_swap_chain // VkSwapchainKHR                 oldSwapchain
    };
    // Enable transfer source on swap chain images if supported
    if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
    {
        swap_chain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    // Enable transfer destination on swap chain images if supported
    if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
    {
        swap_chain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    ret->Value = VK_NULL_HANDLE;
#ifdef VK_EXT_full_screen_exclusive
    bool supportsExclusiveFullscreen = false;
    if (full_screen_exclusive && VKEW_EXT_full_screen_exclusive)
    {
        VkPhysicalDeviceSurfaceInfo2KHR physicalDeviceSurfaceInfo2KHR = {  };
        VkSurfaceFullScreenExclusiveInfoEXT surfaceFullScreenExclusiveInfoEXT = {  };
        VkSurfaceCapabilities2KHR surfaceCapabilities2KHR = {  };
        // Ensure Vulkan surface capabilities are queried before enabling fullscreen exclusive
        surfaceCapabilities2KHR.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
        surfaceCapabilities2KHR.pNext = NULL;
        physicalDeviceSurfaceInfo2KHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;

        physicalDeviceSurfaceInfo2KHR.surface = presentationSurface->Value;
        VkResult capabilityResult = vkGetPhysicalDeviceSurfaceCapabilities2KHR(
            physicalDevice,
            &physicalDeviceSurfaceInfo2KHR,
            &surfaceCapabilities2KHR
        );
        supportsExclusiveFullscreen = (capabilityResult == VK_SUCCESS);
        if (supportsExclusiveFullscreen)
        {
            VkSurfaceFullScreenExclusiveWin32InfoEXT surfaceFullScreenExclusiveWin32InfoEXT = {};
            surfaceFullScreenExclusiveWin32InfoEXT.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
            surfaceFullScreenExclusiveWin32InfoEXT.hmonitor = MonitorFromWindow(static_cast<HWND>(platformWindow), MONITOR_DEFAULTTOPRIMARY);
            surfaceFullScreenExclusiveWin32InfoEXT.pNext = NULL;
            surfaceFullScreenExclusiveInfoEXT.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
            surfaceFullScreenExclusiveInfoEXT.fullScreenExclusive = VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT;
            surfaceFullScreenExclusiveInfoEXT.pNext = &surfaceFullScreenExclusiveWin32InfoEXT;
            swap_chain_create_info.pNext = &surfaceFullScreenExclusiveInfoEXT;
        }
    }
    // Attempt to create the swapchain
    VkResult result = vkCreateSwapchainKHR(device, &swap_chain_create_info, vkewGetAllocationCallbacks(), &ret->Value);
    if (result != VK_SUCCESS && supportsExclusiveFullscreen)
    {
        // Retry without fullscreen exclusive if the first attempt failed
        swap_chain_create_info.pNext = NULL;
        result = vkCreateSwapchainKHR(device, &swap_chain_create_info, vkewGetAllocationCallbacks(), &ret->Value);
    }
#endif
    if (ret->Value == VK_NULL_HANDLE)
    {
        if (VK_CHECK(
            vkCreateSwapchainKHR(device, &swap_chain_create_info, vkewGetAllocationCallbacks(), &ret->Value)) !=
            VK_SUCCESS)
        {
            return nullptr;
        }
    }

    ret->Format = desired_format.format;
    uint32_t image_count = 0;
    if ((vkGetSwapchainImagesKHR(device, ret->Value, &image_count, NULL) != VK_SUCCESS) ||
        (image_count == 0))
    {
        return nullptr;
    }
    VkImage* images = new VkImage[image_count];
    if (vkGetSwapchainImagesKHR(device, ret->Value, &image_count, images) != VK_SUCCESS)
    {
        return nullptr;
    }

    for (int i = 0; i < (int)image_count; i++)
    {
        ImageRef t = std::make_shared<Image>();
        t->Value = images[i];
        ret->Images.push_back(t);

        VkImageViewCreateInfo image_view_create_info = {
           VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, // VkStructureType                sType
           NULL, // const void                    *pNext
           0, // VkImageViewCreateFlags         flags
           t->Value, // VkImage                        image
           VK_IMAGE_VIEW_TYPE_2D, // VkImageViewType                viewType
           colorFormat, // VkFormat                       format
           {
                // VkComponentMapping             components
                VK_COMPONENT_SWIZZLE_IDENTITY, // VkComponentSwizzle             r
                VK_COMPONENT_SWIZZLE_IDENTITY, // VkComponentSwizzle             g
                VK_COMPONENT_SWIZZLE_IDENTITY, // VkComponentSwizzle             b
                VK_COMPONENT_SWIZZLE_IDENTITY // VkComponentSwizzle             a
            },
            {
                // VkImageSubresourceRange        subresourceRange
                VK_IMAGE_ASPECT_COLOR_BIT, // VkImageAspectFlags             aspectMask
                0, // uint32_t                       baseMipLevel
                1, // uint32_t                       levelCount
                0, // uint32_t                       baseArrayLayer
                1 // uint32_t                       layerCount
            }
        };

        auto imgView = std::make_shared<ImageView>();
        imgView->Create(image_view_create_info);
        ret->ImageViews.push_back(imgView);

    }
    delete[] images;

    ret->Extent = desired_extent;

    delete[] surface_formats;
    delete[] present_modes;

    return ret;

}
SwapChain::~SwapChain()
{
    for (auto& image : Images)
    {
        image->Value = VK_NULL_HANDLE;
    }
    Images.clear();
    if (Value)
    {
        vkDestroySwapchainKHR(vkewGetDevice(), Value, vkewGetAllocationCallbacks());
        Value = VK_NULL_HANDLE;
    }
}

VkResult SwapChain::AcquireFullScreenExclusiveMode()
{
#ifdef VK_EXT_full_screen_exclusive
    if (VKEW_EXT_full_screen_exclusive)
    {
        return vkAcquireFullScreenExclusiveModeEXT(vkewGetDevice(), Value);
    }
#endif
    return VK_ERROR_FEATURE_NOT_PRESENT;
}

VkResult SwapChain::ReleaseFullScreenExclusiveMode()
{
#ifdef VK_EXT_full_screen_exclusive
    if (VKEW_EXT_full_screen_exclusive)
    {
        return vkReleaseFullScreenExclusiveModeEXT(vkewGetDevice(), Value);
    }
#endif
    return VK_ERROR_FEATURE_NOT_PRESENT;
}


VkResult SwapChain::GetSwapchainImages(uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    return vkGetSwapchainImagesKHR(vkewGetDevice(), Value, pSwapchainImageCount, pSwapchainImages);
}

VkResult SwapChain::AcquireNextImage(uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    return vkAcquireNextImageKHR(vkewGetDevice(), Value, timeout, semaphore, fence, pImageIndex);
}

VkResult SwapChain::WaitForPresent(uint64_t presentId, uint64_t timeout)
{
    if (VKEW_KHR_present_wait)
    {
        return vkWaitForPresentKHR(vkewGetDevice(), Value, presentId, timeout);

    }
    return VK_SUCCESS;
}

// gfx_impl_vk_surface.cpp

std::shared_ptr<Surface> Surface::Create(VkInstance instance, void* platformHandle, void* platformWindow)
{
    auto ret = std::make_shared<Surface>();

    VkResult result;
#if defined(IS_PLATFORM_WIN)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = (HINSTANCE)platformHandle; // provided by the platform code
    surfaceCreateInfo.hwnd = (HWND)platformWindow; // provided by the platform code
    result = VK_CHECK(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, vkewGetAllocationCallbacks(), &ret->Value));
#elif defined(IS_PLATFORM_ANDROID)
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo;
    memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.window = (struct ANativeWindow*)platformWindow;
    result = vkCreateAndroidSurfaceKHR(instance, &surfaceCreateInfo, vkewGetAllocationCallbacks(), &ret->Value);
#elif defined (IS_PLATFORM_NX)
    VkViSurfaceCreateInfoNN surfaceCreateInfo;
    memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_VI_SURFACE_CREATE_INFO_NN;
    surfaceCreateInfo.window = platformWindow;
    result = vkCreateViSurfaceNN(platformHandle, &surfaceCreateInfo, vkewGetAllocationCallbacks(), &ret->Value);
#elif defined (IS_PLATFORM_UNIX)
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
    memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.connection = platformHandle;               // provided by the platform code
    surfaceCreateInfo.window = platformWindow;                       // provided by the platform code
    result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, vkewGetAllocationCallbacks(), &ret->Value);
#else
    error("Unsupported platform");
#endif
    if (result != VK_SUCCESS)
    {
        return nullptr;
    }

    return ret;
}


// Implementation

void Surface::GetPhysicalDeviceSurfaceFormats(VkPhysicalDevice physicalDevice)
{
    if (Value == VK_NULL_HANDLE) return;

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Value, &formatCount, nullptr);

    if (formatCount > 0)
    {
        formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Value, &formatCount, formats.data());
    }
}

#ifdef VK_KHR_display

std::vector<DisplayInfo> Surface::EnumerateDisplays(VkPhysicalDevice physicalDevice)
{
    std::vector<DisplayInfo> displays;

    uint32_t displayCount = 0;
    if (vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &displayCount, nullptr) != VK_SUCCESS)
        return displays;

    if (displayCount == 0)
        return displays;

    std::vector<VkDisplayPropertiesKHR> displayProps(displayCount);
    if (vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &displayCount, displayProps.data()) != VK_SUCCESS)
        return displays;

    displays.reserve(displayCount);
    for (const auto& prop : displayProps)
    {
        DisplayInfo info;
        info.name = prop.displayName ? prop.displayName : "Unknown Display";
        info.width = prop.physicalResolution.width;
        info.height = prop.physicalResolution.height;
        info.display = prop.display;
        info.properties = prop;
        info.modes = EnumerateDisplayModes(physicalDevice, prop.display);

        displays.push_back(std::move(info));
    }

    return displays;
}

std::vector<DisplayMode> Surface::EnumerateDisplayModes(
    VkPhysicalDevice physicalDevice,
    VkDisplayKHR display)
{
    std::vector<DisplayMode> modes;

    uint32_t modeCount = 0;
    if (vkGetDisplayModePropertiesKHR(physicalDevice, display, &modeCount, nullptr) != VK_SUCCESS)
        return modes;

    if (modeCount == 0)
        return modes;

    std::vector<VkDisplayModePropertiesKHR> modeProps(modeCount);
    if (vkGetDisplayModePropertiesKHR(physicalDevice, display, &modeCount, modeProps.data()) != VK_SUCCESS)
        return modes;

    modes.reserve(modeCount);
    for (const auto& prop : modeProps)
    {
        DisplayMode mode;
        mode.width = prop.parameters.visibleRegion.width;
        mode.height = prop.parameters.visibleRegion.height;
        mode.refreshRate = prop.parameters.refreshRate;
        mode.display = display;
        mode.mode = prop.displayMode;
        mode.properties = prop;

        modes.push_back(mode);
    }

    return modes;
}

std::optional<DisplayMode> Surface::FindDisplayMode(
    VkPhysicalDevice physicalDevice,
    uint32_t width,
    uint32_t height,
    uint32_t minRefreshRate)
{
    auto displays = EnumerateDisplays(physicalDevice);

    std::optional<DisplayMode> bestMatch;
    uint32_t bestRefreshRate = minRefreshRate;

    for (const auto& display : displays)
    {
        for (const auto& mode : display.modes)
        {
            if (mode.width == width && mode.height == height && mode.refreshRate >= minRefreshRate)
            {
                if (!bestMatch || mode.refreshRate > bestRefreshRate)
                {
                    bestMatch = mode;
                    bestRefreshRate = mode.refreshRate;
                }
            }
        }
    }

    return bestMatch;
}

std::optional<DisplayMode> Surface::GetBestDisplayMode(VkPhysicalDevice physicalDevice)
{
    auto displays = EnumerateDisplays(physicalDevice);

    std::optional<DisplayMode> best;

    for (const auto& display : displays)
    {
        for (const auto& mode : display.modes)
        {
            if (!best ||
                (mode.width * mode.height > best->width * best->height) ||
                (mode.width * mode.height == best->width * best->height && mode.refreshRate > best->refreshRate))
            {
                best = mode;
            }
        }
    }

    return best;
}

std::optional<DisplayMode> Surface::GetNativeDisplayMode(VkPhysicalDevice physicalDevice)
{
    auto displays = EnumerateDisplays(physicalDevice);

    if (displays.empty() || displays[0].modes.empty())
        return std::nullopt;

    // First mode is typically the native resolution
    return displays[0].modes[0];
}

std::optional<uint32_t> Surface::FindCompatiblePlane(
    VkPhysicalDevice physicalDevice,
    VkDisplayKHR display)
{
    uint32_t planeCount = 0;
    if (vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &planeCount, nullptr) != VK_SUCCESS)
        return std::nullopt;

    if (planeCount == 0)
        return std::nullopt;

    std::vector<VkDisplayPlanePropertiesKHR> planeProps(planeCount);
    if (vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &planeCount, planeProps.data()) != VK_SUCCESS)
        return std::nullopt;

    for (uint32_t i = 0; i < planeCount; ++i)
    {
        uint32_t supportedCount = 0;
        if (vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, i, &supportedCount, nullptr) != VK_SUCCESS)
            continue;

        if (supportedCount == 0)
            continue;

        std::vector<VkDisplayKHR> supportedDisplays(supportedCount);
        if (vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, i, &supportedCount, supportedDisplays.data()) != VK_SUCCESS)
            continue;

        for (const auto& supportedDisplay : supportedDisplays)
        {
            if (supportedDisplay == display)
                return i;
        }
    }

    return std::nullopt;
}

std::shared_ptr<Surface> Surface::CreateFullscreen(
    VkInstance instance,
    VkPhysicalDevice physicalDevice,
    const DisplayMode& displayMode)
{
    auto surface = std::make_shared<Surface>();

    // Find compatible plane
    auto planeIndex = FindCompatiblePlane(physicalDevice, displayMode.display);
    if (!planeIndex)
        return nullptr;

    // Get plane properties for stack index
    uint32_t planeCount = 0;
    vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &planeCount, nullptr);
    std::vector<VkDisplayPlanePropertiesKHR> planeProps(planeCount);
    vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &planeCount, planeProps.data());

    // Create display surface
    VkDisplaySurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.displayMode = displayMode.mode;
    surfaceCreateInfo.planeIndex = *planeIndex;
    surfaceCreateInfo.planeStackIndex = planeProps[*planeIndex].currentStackIndex;
    surfaceCreateInfo.transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    surfaceCreateInfo.globalAlpha = 1.0f;
    surfaceCreateInfo.alphaMode = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR;
    surfaceCreateInfo.imageExtent = displayMode.properties.parameters.visibleRegion;

    VkResult result = vkCreateDisplayPlaneSurfaceKHR(
        instance,
        &surfaceCreateInfo,
        vkewGetAllocationCallbacks(),
        &surface->Value);

    if (result != VK_SUCCESS)
        return nullptr;

    return surface;
}

#endif // VK_KHR_display


Image::~Image()
{
    // Image destruction is handled by the swapchain
	vkDestroyImage(vkewGetDevice(), Value, vkewGetAllocationCallbacks());
}   

VkResult Image::Create(const VkImageCreateInfo& create_info)
{
    return vkCreateImage(vkewGetDevice(), &create_info, vkewGetAllocationCallbacks(), &Value);
}
void Image::GetImageMemoryRequirements(VkMemoryRequirements& memRequirements) const
{
    vkGetImageMemoryRequirements(vkewGetDevice(), Value, &memRequirements);
}
void Image::GetImageSubresourceLayout(const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    vkGetImageSubresourceLayout(vkewGetDevice(), Value, pSubresource, pLayout);
}
ImageView::~ImageView()
{
    if (Value != VK_NULL_HANDLE)
    {
        vkDestroyImageView(vkewGetDevice(), Value, vkewGetAllocationCallbacks());
        Value = VK_NULL_HANDLE;
    }
}
VkResult ImageView::Create(const VkImageViewCreateInfo& create_info)
{
    return vkCreateImageView(vkewGetDevice(), &create_info, vkewGetAllocationCallbacks(), &Value);
}