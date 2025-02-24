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
#ifndef NX
#define VK_NO_PROTOTYPES 1
#endif
#include "vulkan/vulkan.h"
#if defined _WIN32 && defined _INC_WINDOWS
#include "vulkan/vulkan_win32.h"
#endif
typedef struct vkewContext VKEWContext;
#ifdef __cplusplus
extern "C" {
#endif
extern int vkewInit(const char* pApplicationName, const char* pEngineName, int apiVersion, int enableValidation, int enableRaytracing);
extern void vkewDestroy(void);
extern void vkewOnErrors(void);
extern void vkewLogMessage(const char* pszFormat, ...);

extern VkDevice vkewGetDevice(void);
extern VkExtent2D vkewGetSwapChainExtent(const VkSurfaceCapabilitiesKHR* surface_capabilities);
extern VkExtent2D vkewGetSwapChainExtent2D(void);
extern VkFormat vkewGetColorFormat(void);
extern VkImageUsageFlags vkewGetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR* surface_capabilities);
extern VkImage vkewGetSwapChainImage(int i);
extern VkInstance vkewGetInstance(void);
extern VkPhysicalDevice vkewGetPhysicalDevice(void);
extern VkPresentModeKHR vkewGetSwapChainPresentMode(const VkPresentModeKHR* present_modes, int count, int vsync);
extern VkQueue vkewGetGraphicsQueue(void);
extern VkQueue vkewGetTransferQueue(void);
extern VkQueue vkewGetComputeQueue(void);
extern VkResult vkewCheckExtensionAvailability(const char* extension_name);

extern VkResult vkewCreateSurface(int deviceIndex, void* platformHandle, void* platformWindow, VkFormat colorFormat,
                                  VkColorSpaceKHR colorSpace);
extern VkResult vkewCreateSwapChain(void* platformWindow, int vsync, VkExtent2D desired_extent,
                                    int full_screen_exclusive, VkFormat colorFormat, VkColorSpaceKHR colorSpace);
extern int vkewGetQueueNodeIndex(void);
extern int vkewGetQueueTransfertIndex(void);
extern int vkewGetComputeIndex(void);
extern VkSurfaceFormatKHR vkewGetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count,
                                                 VkFormat colorFormat, VkColorSpaceKHR colorSpace);
extern VkSurfaceKHR vkewGetPresentationSurface();
extern VkSurfaceTransformFlagBitsKHR vkewGetSwapChainTransform(const VkSurfaceCapabilitiesKHR* surface_capabilities);
extern VkSwapchainKHR vkewGetSwapChain(void);
extern uint32_t vkewGetGraphicsQueueFamilyIndex(void);
extern uint32_t vkewGetComputeQueueFamilyIndex(void);
extern uint32_t vkewGetTransfertQueueFamilyIndex(void);
extern int vkewGetSwapChainCount(void);
extern int vkewSupportsFullscreenExclusive(void);
extern uint32_t vkewGetSwapChainNumImages(const VkSurfaceCapabilitiesKHR* surface_capabilities);
extern void vkewGetPropertiesAndFeatures(VkPhysicalDeviceProperties* p, VkPhysicalDeviceFeatures* f);
extern void vkewGetRaytracingPropertiesAndFeatures(VkPhysicalDeviceRayTracingPipelinePropertiesKHR* p, VkPhysicalDeviceAccelerationStructureFeaturesKHR* f);
extern void vkewReleaseSwapChain(void);

#if defined VK_NO_PROTOTYPES

extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
extern PFN_vkAllocateMemory vkAllocateMemory;
extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
extern PFN_vkBindBufferMemory vkBindBufferMemory;
extern PFN_vkBindBufferMemory2 vkBindBufferMemory2;
extern PFN_vkBindImageMemory vkBindImageMemory;
extern PFN_vkBindImageMemory2 vkBindImageMemory2;
extern PFN_vkCmdBeginQuery vkCmdBeginQuery;
extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
extern PFN_vkCmdBlitImage vkCmdBlitImage;
extern PFN_vkCmdClearAttachments vkCmdClearAttachments;
extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
extern PFN_vkCmdCopyImage vkCmdCopyImage;
extern PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
extern PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
extern PFN_vkCmdDispatch vkCmdDispatch;
extern PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
extern PFN_vkCmdDraw vkCmdDraw;
extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
extern PFN_vkCmdEndQuery vkCmdEndQuery;
extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
extern PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
extern PFN_vkCmdPushConstants vkCmdPushConstants;
extern PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
extern PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
extern PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
extern PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
extern PFN_vkCmdSetScissor vkCmdSetScissor;
extern PFN_vkCmdSetViewport vkCmdSetViewport;
extern PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
extern PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
extern PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
extern PFN_vkCreateBuffer vkCreateBuffer;
extern PFN_vkCreateBufferView vkCreateBufferView;
extern PFN_vkCreateCommandPool vkCreateCommandPool;
extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkCreateFence vkCreateFence;
extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
extern PFN_vkCreateImage vkCreateImage;
extern PFN_vkCreateImageView vkCreateImageView;
extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
extern PFN_vkCreateQueryPool vkCreateQueryPool;
extern PFN_vkCreateRenderPass vkCreateRenderPass;
extern PFN_vkCreateSampler vkCreateSampler;
extern PFN_vkCreateSemaphore vkCreateSemaphore;
extern PFN_vkCreateShaderModule vkCreateShaderModule;
extern PFN_vkDestroyBuffer vkDestroyBuffer;
extern PFN_vkDestroyBufferView vkDestroyBufferView;
extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
extern PFN_vkDestroyDevice vkDestroyDevice;
extern PFN_vkDestroyFence vkDestroyFence;
extern PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
extern PFN_vkDestroyImage vkDestroyImage;
extern PFN_vkDestroyImageView vkDestroyImageView;
extern PFN_vkDestroyInstance vkDestroyInstance;
extern PFN_vkDestroyPipeline vkDestroyPipeline;
extern PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
extern PFN_vkDestroyRenderPass vkDestroyRenderPass;
extern PFN_vkDestroySampler vkDestroySampler;
extern PFN_vkDestroySemaphore vkDestroySemaphore;
extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
extern PFN_vkFreeMemory vkFreeMemory;
extern PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
extern PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
extern PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
extern PFN_vkGetFenceStatus vkGetFenceStatus;
extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
extern PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
extern PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
extern PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
extern PFN_vkMapMemory vkMapMemory;
extern PFN_vkQueuePresentKHR vkQueuePresentKHR;
extern PFN_vkQueueSubmit vkQueueSubmit;
extern PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
extern PFN_vkResetCommandPool vkResetCommandPool;
extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
extern PFN_vkResetFences vkResetFences;
extern PFN_vkUnmapMemory vkUnmapMemory;
extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
extern PFN_vkWaitForFences vkWaitForFences;
extern PFN_vkWaitSemaphores vkWaitSemaphores;
#endif
#if defined VK_NO_PROTOTYPES
extern PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
extern PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
#endif
#ifdef VK_KHR_swapchain
extern VkBool32 VKEW_KHR_swapchain;
extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
extern PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
#endif

#ifdef VK_KHR_display
extern VkBool32 VKEW_KHR_display;
#if defined VK_NO_PROTOTYPES
extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
extern PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
extern PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
extern PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
extern PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
#endif
#endif

#ifdef VK_KHR_image_format_list
extern VkBool32 VKEW_KHR_image_format_list;
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
extern VkBool32 VKEW_KHR_synchronization2;
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
#ifdef VK_EXT_robustness2
extern VkBool32 VKEW_EXT_robustness2;
#endif

#ifdef VK_KHR_get_physical_device_properties2 
extern VkBool32 VKEW_KHR_get_physical_device_properties2;
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

#ifdef VK_KHR_push_descriptor 
extern VkBool32 VKEW_KHR_push_descriptor;
#if defined VK_NO_PROTOTYPES
extern PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
extern PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
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

#ifdef VK_KHR_maintenance1
extern VkBool32 VKEW_KHR_maintenance1;
#endif

#ifdef VK_KHR_maintenance2
extern VkBool32 VKEW_KHR_maintenance2;

#ifdef VK_KHR_maintenance3
extern VkBool32 VKEW_KHR_maintenance3;
#endif

#ifdef VK_KHR_maintenance4
extern VkBool32 VKEW_KHR_maintenance4;
extern PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
extern PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
extern PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif

#ifdef VK_KHR_maintenance5
extern VkBool32 VKEW_KHR_maintenance5;
extern PFN_vkCmdBindIndexBuffer2KHR vkCmdBindIndexBuffer2KHR;
extern PFN_vkGetRenderingAreaGranularityKHR vkGetRenderingAreaGranularityKHR;
extern PFN_vkGetDeviceImageSubresourceLayoutKHR vkGetDeviceImageSubresourceLayoutKHR;
extern PFN_vkGetImageSubresourceLayout2KHR vkGetImageSubresourceLayout2KHR;
#endif


// VK_KHR_acceleration_structure is a preprocessor guard. Do not pass it to API calls.
#ifdef VK_KHR_acceleration_structure
extern VkBool32 VKEW_KHR_acceleration_structure;
extern PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
extern PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
extern PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
extern PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
extern PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
extern PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
extern PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
extern PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
extern PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
extern PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
extern PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
extern PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
extern PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
extern PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
extern PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
extern PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
#endif

#ifdef VK_KHR_ray_tracing_pipeline
extern VkBool32 VKEW_KHR_ray_tracing_pipeline;
extern PFN_vkCmdTraceRaysKHR                                 vkCmdTraceRaysKHR;
extern PFN_vkCreateRayTracingPipelinesKHR                    vkCreateRayTracingPipelinesKHR;
extern PFN_vkGetRayTracingShaderGroupHandlesKHR              vkGetRayTracingShaderGroupHandlesKHR;
extern PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
extern PFN_vkCmdTraceRaysIndirectKHR                         vkCmdTraceRaysIndirectKHR;
extern PFN_vkGetRayTracingShaderGroupStackSizeKHR            vkGetRayTracingShaderGroupStackSizeKHR;
extern PFN_vkCmdSetRayTracingPipelineStackSizeKHR            vkCmdSetRayTracingPipelineStackSizeKHR;
#endif 

#ifdef VK_KHR_ray_query
extern VkBool32 VKEW_KHR_ray_query;
extern PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT;
extern PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT;
extern PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT;
#endif 

#ifdef VK_KHR_buffer_device_address
extern VkBool32 VKEW_KHR_buffer_device_address;
#endif

#ifdef VK_KHR_deferred_host_operations
extern VkBool32 VKEW_KHR_deferred_host_operations;
#endif

#ifdef VK_EXT_descriptor_indexing
extern VkBool32 VKEW_EXT_descriptor_indexing;
#endif

#ifdef VK_KHR_spirv_1_4
extern VkBool32 VKEW_KHR_spirv_1_4;
#endif

#ifdef VK_KHR_shader_float_controls
extern VkBool32 VKEW_KHR_shader_float_controls;
#endif

#ifdef VK_KHR_pipeline_library 
extern VkBool32 VKEW_KHR_pipeline_library;
#endif

extern VkBool32 VKEW_VERSION_1_1;
extern VkBool32 VKEW_VERSION_1_2;
extern VkBool32 VKEW_VERSION_1_3;
extern VkBool32 VKEW_VERSION_1_4;



#ifdef __cplusplus
}
#endif
#endif
#endif