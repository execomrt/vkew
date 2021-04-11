/*
 ** The Vulkan Extension Wrangler Library
 ** Copyright (C) 2020 Stephane Denis
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are met:
 **
 ** * Redistributions of source code must retain the above copyright notice,
 **   this list of conditions and the following disclaimer.
 ** * Redistributions in binary form must reproduce the above copyright notice,
 **   this list of conditions and the following disclaimer in the documentation
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
 */

#ifndef __vkew_h__
#define __vkew_h__
#define __VKEW_H__

#define VK_NO_PROTOTYPES 1

#include "vulkan/vulkan.h"

#if defined _WIN32 && defined _INC_WINDOWS
#include "vulkan/vulkan_win32.h"
#endif

typedef struct vkewContext VKEWContext;


#ifdef __cplusplus
extern "C" {
#endif

	extern VkResult vkewCreateSwapChain(void* platformWindow, int vsync, VkExtent2D desired_extent, int full_screen_exclusive, VkFormat colorFormat);
	extern VkBool32 vkewCreateSwapChainImageViews(void);
	extern VkCommandPool vkewGetPresentQueuePool(void);
	extern VkDevice vkewGetDevice(void);
	extern VkExtent2D vkewGetSwapChainExtent(const VkSurfaceCapabilitiesKHR* surface_capabilities);
	extern VkExtent2D vkewGetSwapChainExtent2D(void);
	extern VkFormat vkewGetColorFormat(void);
	extern VkImageUsageFlags vkewGetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR* surface_capabilities);
	extern VkImageView vkewGetSwapChainImageView(int i);
	extern VkInstance vkewGetInstance(void);
	extern VkPhysicalDevice vkewGetPhysicalDevice(void);
	extern VkPresentModeKHR vkewGetSwapChainPresentMode(const VkPresentModeKHR* present_modes, int count, int vsync);
	extern VkQueue vkewGetGraphicsQueue(void);
	extern VkResult vkewCheckExtensionAvailability(const char* extension_name);
	extern VkResult vkewCreateCommandPool(void);
	extern VkResult vkewCreateSurface(int deviceIndex, void* platformHandle, void* platformWindow, VkFormat colorFormat);
	extern VkResult vkewDestroyCommandPool(void);
	extern VkSurfaceFormatKHR vkewGetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count, VkFormat colorFormat);
	extern VkSurfaceKHR vkewGetPresentationSurface();
	extern VkSurfaceTransformFlagBitsKHR vkewGetSwapChainTransform(const VkSurfaceCapabilitiesKHR* surface_capabilities);
	extern VkSwapchainKHR vkewGetSwapChain(void);
	extern int vkewGetFrameIndex(void);
	extern int vkewGetGraphicsQueueFamilyIndex(void);
	extern int vkewGetSwapChainCount(void);
	extern int vkewInit(const char* pApplicationName, const char* pEngineName, int apiVersion, int enableValidation);
	extern int vkewSupportsFullscreenExclusive(void);
	extern uint32_t vkewGetSwapChainNumImages(const VkSurfaceCapabilitiesKHR* surface_capabilities);
	extern void vkewDestroy(void);
	extern void vkewGetPropertiesAndFeatures(VkPhysicalDeviceProperties* p, VkPhysicalDeviceFeatures* f);
	extern void vkewLogMessage(const char* pszFormat, ...);
	extern void vkewReleaseSurface(void);
	extern void vkewReleaseSwapChain(void);
	extern void vkewWaitIdle(void);


#if defined VK_NO_PROTOTYPES




	extern PFN_vkCreateInstance vkCreateInstance;
	extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	extern PFN_vkCreateDevice vkCreateDevice;
	extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
	extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
	extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
	extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
	extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
	extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
	extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
	extern PFN_vkCreateShaderModule vkCreateShaderModule;
	extern PFN_vkCreateBuffer vkCreateBuffer;
	extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
	extern PFN_vkMapMemory vkMapMemory;
	extern PFN_vkUnmapMemory vkUnmapMemory;
	extern PFN_vkBindBufferMemory vkBindBufferMemory;
	extern PFN_vkDestroyBuffer vkDestroyBuffer;
	extern PFN_vkAllocateMemory vkAllocateMemory;
	extern PFN_vkBindImageMemory vkBindImageMemory;
	extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
	extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
	extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
	extern PFN_vkCmdCopyImage vkCmdCopyImage;
	extern PFN_vkCmdBlitImage vkCmdBlitImage;
	extern PFN_vkCmdClearAttachments vkCmdClearAttachments;
	extern PFN_vkCreateSampler vkCreateSampler;
	extern PFN_vkDestroySampler vkDestroySampler;
	extern PFN_vkDestroyImage vkDestroyImage;
	extern PFN_vkFreeMemory vkFreeMemory;
	extern PFN_vkCreateRenderPass vkCreateRenderPass;
	extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
	extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
	extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
	extern PFN_vkCreateImage vkCreateImage;
	extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
	extern PFN_vkCreateImageView vkCreateImageView;
	extern PFN_vkDestroyImageView vkDestroyImageView;
	extern PFN_vkCreateSemaphore vkCreateSemaphore;
	extern PFN_vkDestroySemaphore vkDestroySemaphore;
	extern PFN_vkWaitSemaphores vkWaitSemaphores;
	extern PFN_vkCreateFence vkCreateFence;
	extern PFN_vkDestroyFence vkDestroyFence;
	extern PFN_vkGetFenceStatus vkGetFenceStatus;
	extern PFN_vkResetFences vkResetFences;
	extern PFN_vkWaitForFences vkWaitForFences;
	extern PFN_vkCreateCommandPool vkCreateCommandPool;
	extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
	extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
	extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
	extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
	extern PFN_vkGetDeviceQueue vkGetDeviceQueue;


	extern PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
	extern PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
	extern PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
	extern PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
	extern PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
	extern PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;


	extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
	extern PFN_vkQueueSubmit vkQueueSubmit;
	extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
	extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
	extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
	extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
	extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
	extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
	extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
	extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
	extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
	extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;

	extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
	extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
	extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
	extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
	extern PFN_vkCmdSetViewport vkCmdSetViewport;
	extern PFN_vkCmdSetScissor vkCmdSetScissor;
	extern PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
	extern PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
	extern PFN_vkCmdPushConstants vkCmdPushConstants;
	extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;

	extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
	extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;


	extern PFN_vkCmdDraw vkCmdDraw;
	extern PFN_vkCmdDispatch vkCmdDispatch;
	extern PFN_vkDestroyPipeline vkDestroyPipeline;
	extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
	extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
	extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;

	extern PFN_vkDestroyDevice vkDestroyDevice;
	extern PFN_vkDestroyInstance vkDestroyInstance;
	extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
	extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
	extern PFN_vkDestroyRenderPass vkDestroyRenderPass;
	extern PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
	extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
	extern PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
	extern PFN_vkCreateQueryPool vkCreateQueryPool;
	extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
	extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
	extern PFN_vkCmdBeginQuery vkCmdBeginQuery;
	extern PFN_vkCmdEndQuery vkCmdEndQuery;
	extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
	extern PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
	extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
	extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
	extern PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
	extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
	extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
	extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
	extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
	extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
	extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
	extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
	extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
	extern PFN_vkQueuePresentKHR vkQueuePresentKHR;
	extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
	extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;

#endif


#if defined VK_NO_PROTOTYPES
	extern PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
	extern PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
#endif


#ifdef VK_KHR_display
	extern VkBool32 VKEW_KHR_display;
	extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
	extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
	extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
	extern PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
	extern PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
	extern PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
	extern PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
#endif

#ifdef VK_EXT_debug_marker
	extern VkBool32 VKEW_EXT_debug_marker;

#if defined VK_NO_PROTOTYPES
	extern PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
	extern PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
	extern PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
	extern PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
	extern PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif

#endif


#ifdef VK_KHR_synchronization2 

	extern VkBool32 VKEW_KHR_get_surface_capabilities2;
#if defined VK_NO_PROTOTYPES
	extern PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
	extern PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
	extern PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
	extern PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
	extern PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
	extern PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#endif

#endif




#ifdef VK_KHR_get_surface_capabilities2 

	extern VkBool32 VKEW_KHR_get_surface_capabilities2;
#if defined VK_NO_PROTOTYPES
	extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
	extern PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif

#endif

#ifdef VK_EXT_full_screen_exclusive

	extern VkBool32 VKEW_EXT_full_screen_exclusive;
#if defined VK_NO_PROTOTYPES
	extern PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
	extern PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
	extern PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
	extern PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif

#endif



#ifdef VK_EXT_debug_utils
	extern VkBool32 VKEW_EXT_debug_utils;
#if defined VK_NO_PROTOTYPES
	extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
	extern PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
	extern PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
	extern PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
	extern PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
	extern PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
	extern PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
#endif
#endif


#ifdef VK_KHR_xlib_surface

	extern PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
	extern PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;

#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_KHR_xcb_surface

	extern PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
	extern PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;

#endif /* VK_USE_PLATFORM_XCB_KHR */	


#ifdef VK_KHR_wayland_surface

	extern PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
	extern PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR kGetPhysicalDeviceWaylandPresentationSupportKHR;

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_KHR_mir_surface

	extern PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR;
	extern PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR;

#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_KHR_android_surface

	extern PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
	extern int VKEW_KHR_android_surface;

#endif /* VK_USE_PLATFORM_ANDROID_KHR */ 

#ifdef VK_KHR_win32_surface

#if defined VK_NO_PROTOTYPES
	extern PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
	extern PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif

	extern int VKEW_KHR_win32_surface;

#endif /* VK_USE_PLATFORM_WIN32_KHR */

#ifdef _DEBUG
	extern VkResult vkewPrintResult(VkResult result, const char* file, int line);
#define VK_CHECK(result) vkewPrintResult(result, __FILE__, __LINE__)
#else
#define VK_CHECK(result) result
#endif

	extern VkBool32 VKEW_EXT_debug_report;
	extern VkBool32 VKEW_EXT_debug_utils;

#ifdef __cplusplus
}
#endif

#endif
