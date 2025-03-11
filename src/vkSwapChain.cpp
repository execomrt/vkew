/*
 ** The Vulkan Extension Wrangler Library
 ** Copyright (C) 2021, 2025 Stephane Denis
 ** All rights reserved.
 ** V 0.9.8
 **
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are met:
 **
 ** * Redistributions of source code must retain the above copyright notice,
 **   this list of conditions and the following disclaimer.
 ** * Redistributions in binary form must reproduce the above copyright notice,
 **   this list of conditions and the following disclaimer in the documetrntation
 **   and/or other materials provided with the distribution.
 ** * The name of the author may be used to endorse or promote products
 **   derived from this software without specific prior written permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 ** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 ** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 ** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 ** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 ** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 ** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 ** THE POSSIBILITY OF SUCH DAMAGE.
 * 
  */
#ifdef _WIN32
#define IS_PLATFORM_WIN
#endif
#ifdef IS_PLATFORM_WIN
#include <windows.h>
#endif
#include "vkew.h"
#include <iostream>
#include <cstring>
#include <vector>
#define VK_ASSERT(f) { \
    if (!(f)) { \
        std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
}
static VkBool32 useValidationLayer = VK_TRUE;
#include "vkTest.h"

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

VkSurfaceFormatKHR SwapChain::GetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count,
                                                 VkFormat colorFormat,
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

VkExtent2D SwapChain::GetSwapChainExtent(const VkSurfaceCapabilitiesKHR* surface_capabilities)
{
    // Special value of surface extent is width == height == -1
    // If this is so we define the size by ourselves but it must fit within defined confines
    if (surface_capabilities->currentExtent.width == -1)
    {
        VkExtent2D swap_chain_extent = {640, 480};
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

VkImageUsageFlags SwapChain::GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR* surface_capabilities)
{
    if (surface_capabilities->supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
    {
        return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    return -1;
}

VkSurfaceTransformFlagBitsKHR SwapChain::GetSwapChainTransform(const VkSurfaceCapabilitiesKHR* surface_capabilities)
{
    if (surface_capabilities->supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    return surface_capabilities->currentTransform;
}

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
    // FIFO means first-in�first-out � it will queue images you present and swap them one by one in the order given, during the VBLANK interval.
    for (i = 0; i < count; i++)
        if (VK_PRESENT_MODE_FIFO_KHR == present_modes[i])
            return present_modes[i];
    return present_modes[0]; // We need something  ...
}

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
    ret->Value = nullptr;
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
    auto surface_formats = new VkSurfaceFormatKHR[formats_count];
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
    auto present_modes = new VkPresentModeKHR[present_modes_count];
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
    if (static_cast<int>(desired_usage) == -1)
    {
        return nullptr;
    }
    if (static_cast<int>(desired_present_mode) == -1)
    {
        return nullptr;
    }
    if ((desired_extent.width == 0) || (desired_extent.height == 0))
    {
        // Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
        // But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
        return ret;
    }
    VkSurfaceTransformFlagsKHR preTransform;
    if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        preTransform = surface_capabilities.currentTransform;
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
        nullptr, // const void                    *pNext
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
        nullptr, // const uint32_t                *pQueueFamilyIndices
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
        VkPhysicalDeviceSurfaceInfo2KHR physicalDeviceSurfaceInfo2KHR;
        VkSurfaceFullScreenExclusiveInfoEXT surfaceFullScreenExclusiveInfoEXT;
        VkSurfaceCapabilities2KHR surfaceCapabilities2KHR;
        // Ensure Vulkan surface capabilities are queried before enabling fullscreen exclusive
        surfaceCapabilities2KHR.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
        surfaceCapabilities2KHR.pNext = nullptr;
        physicalDeviceSurfaceInfo2KHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
        physicalDeviceSurfaceInfo2KHR.pNext = &surfaceFullScreenExclusiveInfoEXT;
        physicalDeviceSurfaceInfo2KHR.surface = presentationSurface->Value;
        VkResult capabilityResult = vkGetPhysicalDeviceSurfaceCapabilities2KHR(
            physicalDevice,
            &physicalDeviceSurfaceInfo2KHR,
            &surfaceCapabilities2KHR
        );
        supportsExclusiveFullscreen = (capabilityResult == VK_SUCCESS);
        if (supportsExclusiveFullscreen)
        {
#ifdef IS_PLATFORM_WIN
            VkSurfaceFullScreenExclusiveWin32InfoEXT surfaceFullScreenExclusiveWin32InfoEXT = {};
            surfaceFullScreenExclusiveWin32InfoEXT.sType =
                VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
            surfaceFullScreenExclusiveWin32InfoEXT.hmonitor = MonitorFromWindow(
                static_cast<HWND>(platformWindow), MONITOR_DEFAULTTOPRIMARY);
            surfaceFullScreenExclusiveWin32InfoEXT.pNext = nullptr;
            surfaceFullScreenExclusiveInfoEXT.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
            surfaceFullScreenExclusiveInfoEXT.fullScreenExclusive = VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT;
            surfaceFullScreenExclusiveInfoEXT.pNext = &surfaceFullScreenExclusiveWin32InfoEXT;
            swap_chain_create_info.pNext = &surfaceFullScreenExclusiveInfoEXT;
#endif
        }
    }
    // Attempt to create the swapchain
    VkResult result = vkCreateSwapchainKHR(device, &swap_chain_create_info, nullptr, &ret->Value);
    if (result != VK_SUCCESS && supportsExclusiveFullscreen)
    {
        // Retry without fullscreen exclusive if the first attempt failed
        swap_chain_create_info.pNext = nullptr;
        result = vkCreateSwapchainKHR(device, &swap_chain_create_info, nullptr, &ret->Value);
    }
#endif
    if (ret->Value == VK_NULL_HANDLE)
    {
        if (VK_CHECK(
                vkCreateSwapchainKHR(device, &swap_chain_create_info, NULL, &ret->Value)) !=
            VK_SUCCESS)
        {
            return nullptr;
        }
    }
    if (old_swap_chain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(device, old_swap_chain, nullptr);
    }
    ret->Format = desired_format.format;
    uint32_t image_count = 0;
    if ((vkGetSwapchainImagesKHR(device, ret->Value, &image_count, nullptr) != VK_SUCCESS) ||
        (image_count == 0))
    {
        return nullptr;
    }
    auto images = new VkImage[image_count];
    if (vkGetSwapchainImagesKHR(device, ret->Value, &image_count, images) != VK_SUCCESS)
    {
        return nullptr;
    }
    ret->Images.resize(image_count);
    ret->ImageViews.resize(image_count);
    for (int i = 0; i < static_cast<int>(image_count); i++)
    {
        ret->Images[i] = images[i];
        VkImageViewCreateInfo image_view_create_info = {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, // VkStructureType                sType
            nullptr, // const void                    *pNext
            0, // VkImageViewCreateFlags         flags
            images[i], // VkImage                        image
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
        vkCreateImageView(device, &image_view_create_info, nullptr, &ret->ImageViews[i]);
    }
    delete[] images;
    ret->Extent = desired_extent;
    delete[] surface_formats;
    delete[] present_modes;
    return ret;
}

SwapChain::~SwapChain()
{    
    for (auto it : ImageViews)
    {
        vkDestroyImageView(vkewGetDevice(), it, nullptr);
    }
    Images.clear();
    ImageViews.clear();
    if (Value)
    {
        vkDestroySwapchainKHR(vkewGetDevice(), Value, nullptr);
        Value = VK_NULL_HANDLE;
    }
}

std::shared_ptr<Surface> Surface::Create(VkInstance instance, void* platformHandle, void* platformWindow)
{
    auto ret = std::make_shared<Surface>();
    VkResult result;
#if defined(IS_PLATFORM_WIN)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = static_cast<HINSTANCE>(platformHandle); // provided by the platform code
    surfaceCreateInfo.hwnd = static_cast<HWND>(platformWindow); // provided by the platform code
    result = VK_CHECK(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &ret->Value));
#elif defined(IS_PLATFORM_ANDROID)
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.window = platformWindow;
    result = vkCreateAndroidSurfaceKHR(instance, &surfaceCreateInfo, NULL, &ret->Value);
#elif defined (IS_PLATFORM_LINUX)
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.connection = platformHandle;               // provided by the platform code
    surfaceCreateInfo.window = platformWindow;                       // provided by the platform code
    result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, NULL, &ret->Value);
#endif
    if (result != VK_SUCCESS)
    {
        return nullptr;
    }
    return ret;
}

void Surface::GetPhysicalDeviceSurfaceFormats(VkPhysicalDevice physicalDevice)
{
    uint32_t formatCount = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Value, &formatCount, NULL));
    Formats.resize(formatCount);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Value, &formatCount, &Formats[0]));
}
