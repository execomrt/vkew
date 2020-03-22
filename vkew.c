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

#define VK_NO_PROTOTYPES 1
#ifdef _WIN32
/* Windows */
#include <Windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#elif defined __ANDROID__
/* Android */
#include <stddef.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#define VK_USE_PLATFORM_ANDROID_KHR

#else
/* Other */ 
#error "Not supported yet"
#endif

#include "vkew.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// This is incompatible with Render Doc
#define USE_EXT_debug_report
// Default frame buffer format
#define DEFAULT_FRAME_BUFFER_FORMAT VK_FORMAT_B8G8R8A8_UNORM


VkResult vkewQueuePresent(const VkSemaphore* pWaitSemaphores);

#ifdef _DEBUG 


static inline const char* vkObjectType_ToString(VkObjectType input_value)
{
	switch ((VkObjectType)input_value)
	{
	case VK_OBJECT_TYPE_QUERY_POOL:
		return "VK_OBJECT_TYPE_QUERY_POOL";
	case VK_OBJECT_TYPE_OBJECT_TABLE_NVX:
		return "VK_OBJECT_TYPE_OBJECT_TABLE_NVX";
	case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:
		return "VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION";
	case VK_OBJECT_TYPE_SEMAPHORE:
		return "VK_OBJECT_TYPE_SEMAPHORE";
	case VK_OBJECT_TYPE_SHADER_MODULE:
		return "VK_OBJECT_TYPE_SHADER_MODULE";
	case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
		return "VK_OBJECT_TYPE_SWAPCHAIN_KHR";
	case VK_OBJECT_TYPE_SAMPLER:
		return "VK_OBJECT_TYPE_SAMPLER";
	case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX:
		return "VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX";
	case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:
		return "VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT";
	case VK_OBJECT_TYPE_IMAGE:
		return "VK_OBJECT_TYPE_IMAGE";
	case VK_OBJECT_TYPE_UNKNOWN:
		return "VK_OBJECT_TYPE_UNKNOWN";
	case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
		return "VK_OBJECT_TYPE_DESCRIPTOR_POOL";
	case VK_OBJECT_TYPE_COMMAND_BUFFER:
		return "VK_OBJECT_TYPE_COMMAND_BUFFER";
	case VK_OBJECT_TYPE_BUFFER:
		return "VK_OBJECT_TYPE_BUFFER";
	case VK_OBJECT_TYPE_SURFACE_KHR:
		return "VK_OBJECT_TYPE_SURFACE_KHR";
	case VK_OBJECT_TYPE_INSTANCE:
		return "VK_OBJECT_TYPE_INSTANCE";
	case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:
		return "VK_OBJECT_TYPE_VALIDATION_CACHE_EXT";
	case VK_OBJECT_TYPE_IMAGE_VIEW:
		return "VK_OBJECT_TYPE_IMAGE_VIEW";
	case VK_OBJECT_TYPE_DESCRIPTOR_SET:
		return "VK_OBJECT_TYPE_DESCRIPTOR_SET";
	case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
		return "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT";
	case VK_OBJECT_TYPE_COMMAND_POOL:
		return "VK_OBJECT_TYPE_COMMAND_POOL";
	case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
		return "VK_OBJECT_TYPE_PHYSICAL_DEVICE";
	case VK_OBJECT_TYPE_DISPLAY_KHR:
		return "VK_OBJECT_TYPE_DISPLAY_KHR";
	case VK_OBJECT_TYPE_BUFFER_VIEW:
		return "VK_OBJECT_TYPE_BUFFER_VIEW";
	case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:
		return "VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT";
	case VK_OBJECT_TYPE_FRAMEBUFFER:
		return "VK_OBJECT_TYPE_FRAMEBUFFER";
	case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:
		return "VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE";
	case VK_OBJECT_TYPE_PIPELINE_CACHE:
		return "VK_OBJECT_TYPE_PIPELINE_CACHE";
	case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
		return "VK_OBJECT_TYPE_PIPELINE_LAYOUT";
	case VK_OBJECT_TYPE_DEVICE_MEMORY:
		return "VK_OBJECT_TYPE_DEVICE_MEMORY";
	case VK_OBJECT_TYPE_FENCE:
		return "VK_OBJECT_TYPE_FENCE";
	case VK_OBJECT_TYPE_QUEUE:
		return "VK_OBJECT_TYPE_QUEUE";
	case VK_OBJECT_TYPE_DEVICE:
		return "VK_OBJECT_TYPE_DEVICE";
	case VK_OBJECT_TYPE_RENDER_PASS:
		return "VK_OBJECT_TYPE_RENDER_PASS";
	case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
		return "VK_OBJECT_TYPE_DISPLAY_MODE_KHR";
	case VK_OBJECT_TYPE_EVENT:
		return "VK_OBJECT_TYPE_EVENT";
	case VK_OBJECT_TYPE_PIPELINE:
		return "VK_OBJECT_TYPE_PIPELINE";
	default:
		return "Unhandled VkObjectType";
	}
}


static const char* VkResult_ToString(VkResult result) {
	switch (result) {
	case VK_SUCCESS:
		return "VK_SUCCESS";
	case VK_NOT_READY:
		return "VK_NOT_READY";
	case VK_TIMEOUT:
		return "VK_TIMEOUT";
	case VK_EVENT_SET:
		return "VK_EVENT_SET";
	case VK_EVENT_RESET:
		return "VK_EVENT_RESET";
	case VK_INCOMPLETE:
		return "VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:
		return "VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "VK_ERROR_SURFACE_LOST_KHR";
	case VK_SUBOPTIMAL_KHR:
		return "VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_RESULT_RANGE_SIZE:
		return "VK_RESULT_RANGE_SIZE";
	case VK_RESULT_MAX_ENUM:
		return "VK_RESULT_MAX_ENUM";
	case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
		return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
	default:
	{
		static char unknown[32];
		sprintf(unknown, "<unknown> %d", result);
		return unknown;
	}
	}
}

void vkewLogMessage(const char* pszFormat, ...)
{	 
	char s_acBuf[2048];
	va_list args;
	va_start(args, pszFormat);
	vsprintf(s_acBuf, pszFormat, args);
#ifdef _WIN32
	OutputDebugStringA(s_acBuf);
	OutputDebugStringA("\n");	
#elif defined(__ANDROID__)
	__android_log_print(ANDROID_LOG_INFO, "vkew", s_acBuf);
#endif
	va_end(args);
}

VkResult vkewPrintResult(VkResult result, const char* file, int line) {
	if (result != VK_SUCCESS) {
		vkewLogMessage("%s(%d): %s", file, line, VkResult_ToString(result));
	}	
	return result;
}

#else

void vkewLogMessage(const char* pszFormat, ...)
{
#if defined(__ANDROID__)
	char s_acBuf[2048];
	va_list args;
	va_start(args, pszFormat);
	vsprintf(s_acBuf, pszFormat, args);
	__android_log_print(ANDROID_LOG_INFO, "vkew", "%s", s_acBuf);
	va_end(args);
#endif
	
}

VkResult vkewPrintResult(VkResult result, const char* file, int line)
{
	return result;
}

#endif

// http://developer.download.nvidia.com/mobile/shield/assets/Vulkan/UsingtheVulkanAPI_20160216.pdf

struct ImageParameters {
	VkImage                       Handle;
	VkImageView                   View;
	VkSampler                     Sampler;
	VkDeviceMemory                Memory;
};

struct SwapChainParameters {
	VkSwapchainKHR                Handle;
	VkFormat                      Format;
	struct ImageParameters*		  Images;
	int							  ImagesCount;
	VkExtent2D                    Extent;
};

// Max fence for presenting
#define MAX_FENCES 3

struct vkewContext
{
	// Basic
	void* ctx;
	VkApplicationInfo appInfo;
	VkInstanceCreateInfo instanceCreateInfo;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	const char * * enabledExtensions;
	const char * * validationLayerNames;
	VkExtensionProperties* availableExtensions;
	int availableExtensionCount;
	int enabledExtensionCount;
	int validationLayerCount;
	int enableValidation;
	int separatePresentQueue;

	VkInstance instance;
	uint32_t frame_index;
	uint32_t current_buffer;
	uint32_t frame_rendered;

	// Swap chain
	struct SwapChainParameters swapChainParams;
	VkSurfaceFormatKHR* surfaceFormats;
	VkDevice device;
	VkPhysicalDevice physicalDevice;
	VkFormat colorFormat;
	VkColorSpaceKHR colorSpace;
	size_t nodeIndex;
	uint32_t formatCount;

	// Handle to the device graphics queue that command buffers are submitted to
	VkQueue queue;

	uint32_t graphicsQueueFamilyIndex;
	uint32_t presentQueueFamilyIndex;
	uint32_t queueNodeIndex;
	VkSurfaceKHR presentationSurface;
	VkDebugReportCallbackEXT messageCallback;
	
	int presentQueueCmdBuffersCount;
	VkCommandBuffer* presentQueueCmdBuffers;
	VkCommandPool presentQueueCmdPool;

	VkSemaphore presentCompleteSemaphore[MAX_FENCES];
	VkSemaphore renderCompleteSemaphore[MAX_FENCES];
	VkSemaphore imageOwnershipSemaphores[MAX_FENCES];
	VkFence waitFences[MAX_FENCES];
	uint8_t waitFencesSubmitted[MAX_FENCES];

	VkDebugUtilsMessengerEXT debugMessenger;
};


typedef struct vkewContext VKEWContext;

static VKEWContext Vulkan;

static void * vkewGetProcAddress(const char* name)
{
#ifdef WIN32
    return  GetProcAddress((HMODULE)Vulkan.ctx, name);
#elif defined __ANDROID__	
    return dlsym(Vulkan.ctx, (const char*)name);
#else
	return NULL;
#endif

}


VkCommandBuffer vkewGetPresentQueueCommandBuffer(int i) {
	return Vulkan.presentQueueCmdBuffers [i];
}

static int _vkewStrLen (const char *const * s)
{
    int i=0;
    if (s == NULL) return VK_FALSE;
    while (s[i] != '\0') i++;
    return i;
}

static int _vkewStrCLen (const char* s, char c)
{
    int i=0;
    if (s == NULL) return VK_FALSE;
    while (s[i] != '\0' && s[i] != c) i++;
    return (s[i] == '\0' || s[i] == c) ? i : 0;
}


#define _vkewResolveProc(s) s = (void*)vkewGetProcAddress(#s)

#define VKEW_GET_PROC(name) PFN_##name name
#define VK_INSTANCE_LEVEL_FUNCTION(name) PFN_##name name
#define VK_DEVICE_LEVEL_FUNCTION(name) PFN_##name name

#ifdef VK_KHR_android_surface
static int vkewInit_VK_KHR_android_surface(VkDevice device)
{
    VK_DEVICE_LEVEL_FUNCTION(vkCreateAndroidSurfaceKHR);
    return VK_TRUE;
}
#endif


#if defined VK_NO_PROTOTYPES
VKEW_GET_PROC(vkEnumerateInstanceExtensionProperties);
VKEW_GET_PROC(vkEnumerateInstanceLayerProperties);
VKEW_GET_PROC(vkCreateInstance);
VKEW_GET_PROC(vkGetInstanceProcAddr);
VKEW_GET_PROC(vkGetDeviceProcAddr);
VK_INSTANCE_LEVEL_FUNCTION(vkFlushMappedMemoryRanges);
VK_INSTANCE_LEVEL_FUNCTION(vkInvalidateMappedMemoryRanges);
VK_INSTANCE_LEVEL_FUNCTION(vkEnumeratePhysicalDevices);
VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceProperties);
VK_INSTANCE_LEVEL_FUNCTION(vkEnumerateDeviceLayerProperties);
VK_INSTANCE_LEVEL_FUNCTION(vkEnumerateDeviceExtensionProperties);
VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceFeatures);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateDevice);
VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceFormatProperties);
VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdPipelineBarrier);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateShaderModule);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateBuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkGetBufferMemoryRequirements);
VK_INSTANCE_LEVEL_FUNCTION(vkMapMemory);
VK_INSTANCE_LEVEL_FUNCTION(vkUnmapMemory);
VK_INSTANCE_LEVEL_FUNCTION(vkBindBufferMemory);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyBuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkAllocateMemory);
VK_INSTANCE_LEVEL_FUNCTION(vkFreeMemory);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateRenderPass);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdBeginRenderPass);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdEndRenderPass);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdExecuteCommands);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateImage);
VK_INSTANCE_LEVEL_FUNCTION(vkGetImageMemoryRequirements);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateImageView);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyImageView);
VK_INSTANCE_LEVEL_FUNCTION(vkBindImageMemory);
VK_INSTANCE_LEVEL_FUNCTION(vkGetImageSubresourceLayout);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyImage);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdBlitImage);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyImage);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdClearAttachments);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyBuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyBufferToImage);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateSampler);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroySampler);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateSemaphore);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroySemaphore);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateFence);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyFence);
VK_INSTANCE_LEVEL_FUNCTION(vkWaitForFences);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateCommandPool);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyCommandPool);
VK_INSTANCE_LEVEL_FUNCTION(vkAllocateCommandBuffers);
VK_INSTANCE_LEVEL_FUNCTION(vkBeginCommandBuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkEndCommandBuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkGetDeviceQueue);
VK_INSTANCE_LEVEL_FUNCTION(vkQueueSubmit);
VK_INSTANCE_LEVEL_FUNCTION(vkQueueWaitIdle);
VK_INSTANCE_LEVEL_FUNCTION(vkDeviceWaitIdle);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateFramebuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkCreatePipelineCache);
VK_INSTANCE_LEVEL_FUNCTION(vkCreatePipelineLayout);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateGraphicsPipelines);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateComputePipelines);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateDescriptorPool);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateDescriptorSetLayout);
VK_INSTANCE_LEVEL_FUNCTION(vkAllocateDescriptorSets);
VK_INSTANCE_LEVEL_FUNCTION(vkUpdateDescriptorSets);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindDescriptorSets);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindPipeline);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindVertexBuffers);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindIndexBuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetViewport);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetScissor);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetLineWidth);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetDepthBias);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdPushConstants);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdDrawIndexed);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdDraw);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdDrawIndirect);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdDrawIndexedIndirect);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdDispatch);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyPipeline);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyPipelineLayout);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDescriptorSetLayout);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDevice);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyInstance);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDescriptorPool);
VK_INSTANCE_LEVEL_FUNCTION(vkFreeCommandBuffers);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyRenderPass);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyFramebuffer);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyShaderModule);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyPipelineCache);
VK_INSTANCE_LEVEL_FUNCTION(vkCreateQueryPool);
VK_INSTANCE_LEVEL_FUNCTION(vkDestroyQueryPool);
VK_INSTANCE_LEVEL_FUNCTION(vkGetQueryPoolResults);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdBeginQuery);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdEndQuery);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdResetQueryPool);
VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyQueryPoolResults);
VK_DEVICE_LEVEL_FUNCTION(vkGetDeviceQueue);
VK_DEVICE_LEVEL_FUNCTION(vkDeviceWaitIdle);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyDevice);
VK_DEVICE_LEVEL_FUNCTION(vkCreateSemaphore);
VK_DEVICE_LEVEL_FUNCTION(vkCreateCommandPool);
VK_DEVICE_LEVEL_FUNCTION(vkAllocateCommandBuffers);
VK_DEVICE_LEVEL_FUNCTION(vkBeginCommandBuffer);
VK_DEVICE_LEVEL_FUNCTION(vkCmdPipelineBarrier);
VK_DEVICE_LEVEL_FUNCTION(vkCmdClearColorImage);
VK_DEVICE_LEVEL_FUNCTION(vkEndCommandBuffer);
VK_DEVICE_LEVEL_FUNCTION(vkQueueSubmit);
VK_DEVICE_LEVEL_FUNCTION(vkFreeCommandBuffers);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyCommandPool);
VK_DEVICE_LEVEL_FUNCTION(vkDestroySemaphore);
VK_DEVICE_LEVEL_FUNCTION(vkCreateImageView);
VK_DEVICE_LEVEL_FUNCTION(vkCreateRenderPass);
VK_DEVICE_LEVEL_FUNCTION(vkCreateFramebuffer);
VK_DEVICE_LEVEL_FUNCTION(vkCreateShaderModule);
VK_DEVICE_LEVEL_FUNCTION(vkCreatePipelineLayout);
VK_DEVICE_LEVEL_FUNCTION(vkCreateGraphicsPipelines);
VK_DEVICE_LEVEL_FUNCTION(vkCmdBeginRenderPass);
VK_DEVICE_LEVEL_FUNCTION(vkCmdBindPipeline);
VK_DEVICE_LEVEL_FUNCTION(vkCmdDraw);
VK_DEVICE_LEVEL_FUNCTION(vkCmdEndRenderPass);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyShaderModule);;
VK_DEVICE_LEVEL_FUNCTION(vkDestroyPipelineLayout);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyPipeline);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyRenderPass);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyFramebuffer);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyImageView);
VK_DEVICE_LEVEL_FUNCTION(vkCreateFence);
VK_DEVICE_LEVEL_FUNCTION(vkCreateBuffer);
VK_DEVICE_LEVEL_FUNCTION(vkGetBufferMemoryRequirements);
VK_DEVICE_LEVEL_FUNCTION(vkAllocateMemory);
VK_DEVICE_LEVEL_FUNCTION(vkBindBufferMemory);
VK_DEVICE_LEVEL_FUNCTION(vkMapMemory);
VK_DEVICE_LEVEL_FUNCTION(vkFlushMappedMemoryRanges);
VK_DEVICE_LEVEL_FUNCTION(vkUnmapMemory);
VK_DEVICE_LEVEL_FUNCTION(vkCmdSetViewport);
VK_DEVICE_LEVEL_FUNCTION(vkCmdSetScissor);
VK_DEVICE_LEVEL_FUNCTION(vkCmdBindVertexBuffers);
VK_DEVICE_LEVEL_FUNCTION(vkWaitForFences);
VK_DEVICE_LEVEL_FUNCTION(vkResetFences);
VK_DEVICE_LEVEL_FUNCTION(vkFreeMemory);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyBuffer);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyFence);
VK_DEVICE_LEVEL_FUNCTION(vkCmdCopyBuffer);
VK_DEVICE_LEVEL_FUNCTION(vkCreateImage);
VK_DEVICE_LEVEL_FUNCTION(vkGetImageMemoryRequirements);
VK_DEVICE_LEVEL_FUNCTION(vkBindImageMemory);
VK_DEVICE_LEVEL_FUNCTION(vkCreateSampler);
VK_DEVICE_LEVEL_FUNCTION(vkCmdCopyBufferToImage);
VK_DEVICE_LEVEL_FUNCTION(vkCreateDescriptorSetLayout);
VK_DEVICE_LEVEL_FUNCTION(vkCreateDescriptorPool);
VK_DEVICE_LEVEL_FUNCTION(vkAllocateDescriptorSets);
VK_DEVICE_LEVEL_FUNCTION(vkUpdateDescriptorSets);
VK_DEVICE_LEVEL_FUNCTION(vkCmdBindDescriptorSets);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyDescriptorPool);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyDescriptorSetLayout);
VK_DEVICE_LEVEL_FUNCTION(vkDestroySampler);
VK_DEVICE_LEVEL_FUNCTION(vkDestroyImage);




#ifdef VK_KHR_xlib_surface

VK_DEVICE_LEVEL_FUNCTION(vkCreateXlibSurfaceKHR);
VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceXlibPresentationSupportKHR);


#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_KHR_xcb_surface

VK_DEVICE_LEVEL_FUNCTION(vkCreateXcbSurfaceKHR);
VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceXcbPresentationSupportKHR);


#endif /* VK_USE_PLATFORM_XCB_KHR */	


#ifdef VK_KHR_wayland_surface

VK_DEVICE_LEVEL_FUNCTION(vkCreateWaylandSurfaceKHR);
VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceWaylandPresentationSupportKHR);

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_KHR_mir_surface

VK_DEVICE_LEVEL_FUNCTION(vkCreateMirSurfaceKHR);
VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceMirPresentationSupportKHR);

#endif /* VK_USE_PLATFORM_MIR_KHR */

#endif

VkBool32 _vkewCheckExtensionAvailability(const char *extension_name, const VkExtensionProperties* available_extensions, int count) {

	for (int i = 0; i < count; ++i) {
		if (strcmp(available_extensions[i].extensionName, extension_name) == 0) {
			return VK_TRUE;
		}
	}
	return VK_FALSE;
}


VkResult vkewCheckExtensionAvailability(const char *extension_name) {

	return _vkewCheckExtensionAvailability(extension_name, Vulkan.availableExtensions, Vulkan.availableExtensionCount);
}

#undef VKEW_GET_PROC
#undef VK_INSTANCE_LEVEL_FUNCTION
#undef VK_DEVICE_LEVEL_FUNCTION


#define VKEW_GET_PROC(name) name = (PFN_##name)(vkewGetProcAddress(#name))
#define VK_INSTANCE_LEVEL_FUNCTION(name) name = (PFN_##name)(vkGetInstanceProcAddr(instance, #name))
#define VK_DEVICE_LEVEL_FUNCTION(name) name = (PFN_##name)(vkGetDeviceProcAddr(device, #name))


#ifdef VK_EXT_debug_marker
#if defined VK_NO_PROTOTYPES
PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif

VkBool32 VKEW_EXT_debug_marker;

static int vkewInit_VK_EXT_debug_marker(VkInstance instance)
{
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDebugMarkerBeginEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDebugMarkerEndEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDebugMarkerInsertEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkDebugMarkerSetObjectNameEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkDebugMarkerSetObjectTagEXT);
	return vkCmdDebugMarkerBeginEXT!=NULL;
}
#endif




#ifdef VK_KHR_get_surface_capabilities2 

VkBool32 VKEW_KHR_get_surface_capabilities2;

#if defined VK_NO_PROTOTYPES
PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif
static int vkewInit_VK_KHR_get_surface_capabilities2(VkInstance instance)
{
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilities2KHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceFormats2KHR);
	return vkGetPhysicalDeviceSurfaceFormats2KHR != NULL;
}
#endif


#ifdef VK_EXT_full_screen_exclusive
// This extension allows setting of policy for exclusive full-screen behavior for either accessing a particular display with a full-screen
// game/application or for cases like bypassing the display compositor.

VkBool32 VKEW_EXT_full_screen_exclusive;

#if defined VK_NO_PROTOTYPES
PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif
static int vkewInit_VK_EXT_full_screen_exclusive(VkInstance instance)
{
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfacePresentModes2EXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetDeviceGroupSurfacePresentModes2EXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkAcquireFullScreenExclusiveModeEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkReleaseFullScreenExclusiveModeEXT);
	return vkGetPhysicalDeviceSurfacePresentModes2EXT != NULL;
}
#endif



#ifdef VK_EXT_debug_report

VkBool32 VKEW_EXT_debug_report;

#if defined VK_NO_PROTOTYPES
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
#endif

static int vkewInit_VK_EXT_debug_report(VkInstance instance)
{
#if defined VK_NO_PROTOTYPES
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDebugReportCallbackEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateDebugReportCallbackEXT);
#endif
	return vkCreateDebugReportCallbackEXT!=NULL;
}

#endif

#ifdef VK_EXT_debug_utils

VkBool32 VKEW_EXT_debug_utils;

#if defined VK_NO_PROTOTYPES
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
#endif

static int vkewInit_VK_EXT_debug_utils(VkInstance instance)
{
#if defined VK_NO_PROTOTYPES
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateDebugUtilsMessengerEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDebugUtilsMessengerEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkSubmitDebugUtilsMessageEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBeginDebugUtilsLabelEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdEndDebugUtilsLabelEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdInsertDebugUtilsLabelEXT);
	VK_INSTANCE_LEVEL_FUNCTION(vkSetDebugUtilsObjectNameEXT);
#endif
	return vkCreateDebugUtilsMessengerEXT!=NULL;
}

#endif


#ifdef VK_KHR_surface

#if defined VK_NO_PROTOTYPES
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
#endif

static int vkewInit_VK_KHR_surface(VkInstance instance)
{
#if defined VK_NO_PROTOTYPES
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroySurfaceKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
#endif
	return VK_TRUE;
}

#endif /* VK_KHR_surface */

#ifdef VK_KHR_swapchain

#if defined VK_NO_PROTOTYPES
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif

static int vkewInit_VK_KHR_swapchain(VkInstance instance)
{		
#if defined VK_NO_PROTOTYPES
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateSwapchainKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroySwapchainKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetSwapchainImagesKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkAcquireNextImageKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkQueuePresentKHR);
#endif
	return VK_TRUE;
}

#endif /* VK_KHR_swapchain  */

#ifdef VK_KHR_display_swapchain

#if defined VK_NO_PROTOTYPES
PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif

static int vkewInit_VK_KHR_display_swapchain(VkInstance instance)
{
#if defined VK_NO_PROTOTYPES
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateSharedSwapchainsKHR);
#endif
	return VK_TRUE;
}

#endif


#ifdef VK_KHR_win32_surface

#if defined VK_NO_PROTOTYPES
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif

static int vkewInit_VK_KHR_win32_surface(VkInstance instance)
{	
#if defined VK_NO_PROTOTYPES
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateWin32SurfaceKHR);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceWin32PresentationSupportKHR);
#endif
	return VK_TRUE;
}

#endif


#ifdef VK_KHR_android_surface

PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;


#endif


#ifdef VK_KHR_xlib_surface
PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif /* VK_USE_PLATFORM_XLIB_KHR */
#ifdef VK_KHR_xcb_surface
PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif /* VK_USE_PLATFORM_XCB_KHR */	
#ifdef VK_KHR_wayland_surface
PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR kGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif /* VK_USE_PLATFORM_WAYLAND_KHR */
#ifdef VK_KHR_mir_surface
PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR;
PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR;
#endif /* VK_USE_PLATFORM_MIR_KHR */
#ifdef VK_KHR_android_surface
PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif /* VK_USE_PLATFORM_ANDROID_KHR */


// Load instance based Vulkan function pointers
int vkewInstanceLevelInit(VkInstance instance)
{
#if defined VK_NO_PROTOTYPES
	VK_INSTANCE_LEVEL_FUNCTION(vkEnumeratePhysicalDevices);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceProperties);
	VK_INSTANCE_LEVEL_FUNCTION(vkEnumerateDeviceLayerProperties);
	VK_INSTANCE_LEVEL_FUNCTION(vkEnumerateDeviceExtensionProperties);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceFeatures);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateDevice);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceFormatProperties);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdPipelineBarrier);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateShaderModule);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateBuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetBufferMemoryRequirements);
	VK_INSTANCE_LEVEL_FUNCTION(vkMapMemory);
	VK_INSTANCE_LEVEL_FUNCTION(vkUnmapMemory);
	VK_INSTANCE_LEVEL_FUNCTION(vkBindBufferMemory);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyBuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkAllocateMemory);
	VK_INSTANCE_LEVEL_FUNCTION(vkFreeMemory);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateRenderPass);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBeginRenderPass);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdEndRenderPass);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdExecuteCommands);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateImage);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetImageMemoryRequirements);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateImageView);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyImageView);
	VK_INSTANCE_LEVEL_FUNCTION(vkBindImageMemory);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetImageSubresourceLayout);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyImage);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBlitImage);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyImage);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdClearAttachments);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyBuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyBufferToImage);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateSampler);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroySampler);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateSemaphore);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroySemaphore);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateFence);
	VK_INSTANCE_LEVEL_FUNCTION(vkResetFences);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyFence);
	VK_INSTANCE_LEVEL_FUNCTION(vkWaitForFences);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateCommandPool);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyCommandPool);
	VK_INSTANCE_LEVEL_FUNCTION(vkAllocateCommandBuffers);
	VK_INSTANCE_LEVEL_FUNCTION(vkBeginCommandBuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkEndCommandBuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetDeviceQueue);
	VK_INSTANCE_LEVEL_FUNCTION(vkQueueSubmit);
	VK_INSTANCE_LEVEL_FUNCTION(vkQueueWaitIdle);
	VK_INSTANCE_LEVEL_FUNCTION(vkDeviceWaitIdle);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateFramebuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreatePipelineCache);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreatePipelineLayout);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateGraphicsPipelines);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateComputePipelines);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateDescriptorPool);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateDescriptorSetLayout);
	VK_INSTANCE_LEVEL_FUNCTION(vkAllocateDescriptorSets);
	VK_INSTANCE_LEVEL_FUNCTION(vkUpdateDescriptorSets);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindDescriptorSets);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindPipeline);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindVertexBuffers);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBindIndexBuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetViewport);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetScissor);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetLineWidth);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdSetDepthBias);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdPushConstants);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDrawIndexed);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDrawIndirect);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDrawIndexedIndirect);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDraw);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdDispatch);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyPipeline);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyPipelineLayout);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDescriptorSetLayout);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDevice);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyInstance);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyDescriptorPool);
	VK_INSTANCE_LEVEL_FUNCTION(vkFreeCommandBuffers);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyRenderPass);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyFramebuffer);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyShaderModule);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyPipelineCache);
	VK_INSTANCE_LEVEL_FUNCTION(vkCreateQueryPool);
	VK_INSTANCE_LEVEL_FUNCTION(vkDestroyQueryPool);
	VK_INSTANCE_LEVEL_FUNCTION(vkGetQueryPoolResults);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdBeginQuery);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdEndQuery);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdResetQueryPool);
	VK_INSTANCE_LEVEL_FUNCTION(vkCmdCopyQueryPoolResults);
	VK_INSTANCE_LEVEL_FUNCTION(vkFlushMappedMemoryRanges);
	VK_INSTANCE_LEVEL_FUNCTION(vkInvalidateMappedMemoryRanges);



#ifdef VK_KHR_win32_surface
	vkewInit_VK_KHR_win32_surface(instance);
#endif /* VK_USE_PLATFORM_WIN32_KHR */


#ifdef VK_KHR_surface
	vkewInit_VK_KHR_surface(instance);
#endif

#ifdef VK_KHR_swapchain
	vkewInit_VK_KHR_swapchain(instance);
#endif

#ifdef VK_KHR_display_swapchain
	vkewInit_VK_KHR_display_swapchain(instance);
#endif


#ifdef VK_EXT_debug_report
	VKEW_EXT_debug_report = vkewInit_VK_EXT_debug_report(instance);
#endif



#ifdef VK_KHR_get_surface_capabilities2
	VKEW_KHR_get_surface_capabilities2 = vkewInit_VK_KHR_get_surface_capabilities2(instance);
#endif


#ifdef VK_EXT_full_screen_exclusive
	VKEW_EXT_full_screen_exclusive = vkewInit_VK_EXT_full_screen_exclusive(instance);
#endif


#ifdef VK_EXT_debug_marker
	VKEW_EXT_debug_marker = vkewInit_VK_EXT_debug_marker(instance);
#endif


	return VK_FALSE;
}

void vkewDestroy()
{	
	if (Vulkan.validationLayerCount) {
#if defined VK_NO_PROTOTYPES
		vkDestroyDebugReportCallbackEXT(vkewGetInstance(), Vulkan.messageCallback, 0);
#endif
		Vulkan.validationLayerCount = 0;
	}

	if (Vulkan.device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(Vulkan.device);

		for (int i = 0; i < Vulkan.swapChainParams.ImagesCount; ++i) {
			if (Vulkan.swapChainParams.Images[i].View != VK_NULL_HANDLE) {
				vkDestroyImageView(Vulkan.device, Vulkan.swapChainParams.Images[i].View, NULL);
			}
		}

		if (Vulkan.swapChainParams.Handle != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(Vulkan.device, Vulkan.swapChainParams.Handle, NULL);
		}
		vkDestroyDevice(Vulkan.device, NULL);
		Vulkan.device = VK_NULL_HANDLE;
	}

	if (Vulkan.presentationSurface != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(Vulkan.instance, Vulkan.presentationSurface, NULL);
		Vulkan.presentationSurface = VK_NULL_HANDLE;
	}

	if (Vulkan.instance != VK_NULL_HANDLE) {
		vkDestroyInstance(Vulkan.instance, NULL);
		Vulkan.instance = VK_NULL_HANDLE;
	}


#if defined(_WIN32)
	FreeLibrary(Vulkan.ctx);
#else
	dlclose(Vulkan.ctx );
#endif
}

VkInstance vkewGetInstance()
{
	return Vulkan.instance;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(
	VkDebugReportFlagsEXT                       msgFlags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     msgCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMsg,
	void*                                       pUserData)
{
	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
 		vkewLogMessage("ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		vkewLogMessage("WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		vkewLogMessage("INFO: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		vkewLogMessage("DEBUG: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
	}

	return VK_TRUE;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	void *pUserData) {

#ifdef _DEBUG
	char prefix[64] = "";
	char *message = (char *)malloc(strlen(pCallbackData->pMessage) + 5000);

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		strcat(prefix, "VERBOSE : ");
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		strcat(prefix, "INFO : ");
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		strcat(prefix, "WARNING : ");
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		strcat(prefix, "ERROR : ");
	}

	if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
		strcat(prefix, "GENERAL");
	}
	else {
		if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
			strcat(prefix, "VALIDATION");
		}
		if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
			if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
				strcat(prefix, "|");
			}
			strcat(prefix, "PERFORMANCE");
		}
	}

	sprintf(message, "%s - Message Id Number: %d | Message Id Name: %s\n\t%s\n", prefix, pCallbackData->messageIdNumber,
		pCallbackData->pMessageIdName, pCallbackData->pMessage);
	if (pCallbackData->objectCount > 0) {
		char tmp_message[500];
		sprintf(tmp_message, "\n\tObjects - %d\n", pCallbackData->objectCount);
		strcat(message, tmp_message);
		for (uint32_t object = 0; object < pCallbackData->objectCount; ++object) {
			if (NULL != pCallbackData->pObjects[object].pObjectName && strlen(pCallbackData->pObjects[object].pObjectName) > 0) {
				sprintf(tmp_message, "\t\tObject[%d] - %s, Handle %p, Name \"%s\"\n", object,
					vkObjectType_ToString(pCallbackData->pObjects[object].objectType),
					(void *)(pCallbackData->pObjects[object].objectHandle), pCallbackData->pObjects[object].pObjectName);
			}
			else {
				sprintf(tmp_message, "\t\tObject[%d] - %s, Handle %p\n", object,
					vkObjectType_ToString(pCallbackData->pObjects[object].objectType),
					(void *)(pCallbackData->pObjects[object].objectHandle));
			}
			strcat(message, tmp_message);
		}
	}
	if (pCallbackData->cmdBufLabelCount > 0) {
		char tmp_message[500];
		sprintf(tmp_message, "\n\tCommand Buffer Labels - %d\n", pCallbackData->cmdBufLabelCount);
		strcat(message, tmp_message);
		for (uint32_t cmd_buf_label = 0; cmd_buf_label < pCallbackData->cmdBufLabelCount; ++cmd_buf_label) {
			sprintf(tmp_message, "\t\tLabel[%d] - %s { %f, %f, %f, %f}\n", cmd_buf_label,
				pCallbackData->pCmdBufLabels[cmd_buf_label].pLabelName, pCallbackData->pCmdBufLabels[cmd_buf_label].color[0],
				pCallbackData->pCmdBufLabels[cmd_buf_label].color[1], pCallbackData->pCmdBufLabels[cmd_buf_label].color[2],
				pCallbackData->pCmdBufLabels[cmd_buf_label].color[3]);
			strcat(message, tmp_message);
		}
	}

#ifdef _WIN32

	vkewLogMessage("%s\n", message);

#elif defined(ANDROID)

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		__android_log_print(ANDROID_LOG_INFO, APP_SHORT_NAME, "%s", message);
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		__android_log_print(ANDROID_LOG_WARN, APP_SHORT_NAME, "%s", message);
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		__android_log_print(ANDROID_LOG_ERROR, APP_SHORT_NAME, "%s", message);
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		__android_log_print(ANDROID_LOG_VERBOSE, APP_SHORT_NAME, "%s", message);
	}
	else {
		__android_log_print(ANDROID_LOG_INFO, APP_SHORT_NAME, "%s", message);
	}

#else

	printf("%s\n", message);
	fflush(stdout);

#endif

	free(message);

#endif

	// Don't bail out, but keep going.
	return VK_FALSE;
}


static VkBool32 _vkewCheckLayerAvailabilty(const uint32_t check_count, const char **check_names, const uint32_t layer_count, const VkLayerProperties *layers) {
	for (uint32_t i = 0; i < check_count; i++) {
		VkBool32 found = 0;
		for (uint32_t j = 0; j < layer_count; j++) {
			if (!strcmp(check_names[i], layers[j].layerName)) {
				found = 1;
				break;
			}
		}
		if (!found) {
			vkewLogMessage("Cannot find layer: %s\n", check_names[i]);
			return 0;
		}
	}
	return 1;
}


static void InitValidationLayers(int enableValidation)
{	
	if (!enableValidation) {
		return;
	}

	uint32_t instance_extension_count = 0;
	uint32_t instance_layer_count = 0;
	uint32_t validation_layer_count = 0;
	int validation_found;


	static const char *instance_validation_layers_alt1[] = { "VK_LAYER_LUNARG_standard_validation" };

	static const char *instance_validation_layers_alt2[] = { "VK_LAYER_GOOGLE_threading", "VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_LUNARG_object_tracker", "VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_GOOGLE_unique_objects" };

		
	VkResult err = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);

	const char **it = instance_validation_layers_alt1;
	if (instance_layer_count > 0) {
		VkLayerProperties *instance_layers = malloc(sizeof(VkLayerProperties) * instance_layer_count);
		err = vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layers);			
		validation_found = _vkewCheckLayerAvailabilty(ARRAY_SIZE(instance_validation_layers_alt1), it,
			instance_layer_count, instance_layers);
		if (validation_found) {
			Vulkan.validationLayerCount = ARRAY_SIZE(instance_validation_layers_alt1);
			Vulkan.validationLayerNames[0] = "VK_LAYER_LUNARG_standard_validation";				
		}
		else {
			// use alternative set of validation layers
			it = instance_validation_layers_alt2;
			Vulkan.validationLayerCount = ARRAY_SIZE(instance_validation_layers_alt2);
			validation_found = _vkewCheckLayerAvailabilty(ARRAY_SIZE(instance_validation_layers_alt2), it,
				instance_layer_count, instance_layers);
			for (int32_t i = 0; i < Vulkan.validationLayerCount; i++) {
				Vulkan.validationLayerNames[i] = it[i];
			}
		}
		free(instance_layers);
	}
}


static void AddExtension(const char* name)
{
	Vulkan.enabledExtensions[Vulkan.enabledExtensionCount] = name;
	Vulkan.enabledExtensionCount++;
}

static void InitExtensionsLayers(int enableValidation)
{
	int platformSurfaceExtFound = 0;
	uint32_t instance_extension_count = 0;
	int surfaceExtFound = 0;
		
	VkResult err = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, NULL);
	
	if (instance_extension_count > 0) {
		VkExtensionProperties *instance_extensions = malloc(sizeof(VkExtensionProperties) * instance_extension_count);
		err = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, instance_extensions);
		
		for (int32_t i = 0; i < (int32_t) instance_extension_count; i++) {
			if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				surfaceExtFound = 1;
				AddExtension(VK_KHR_SURFACE_EXTENSION_NAME);
			}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			if (!strcmp(VK_KHR_XLIB_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtension(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			if (!strcmp(VK_KHR_XCB_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtension(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
			if (!strcmp(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtension(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_MIR_KHR)
#elif defined(VK_USE_PLATFORM_DISPLAY_KHR)
			if (!strcmp(VK_KHR_DISPLAY_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtension(VK_KHR_DISPLAY_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
			if (!strcmp(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtension(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_IOS_MVK)
			if (!strcmp(VK_MVK_IOS_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				demo->extension_names[demo->enabled_extension_count++] = VK_MVK_IOS_SURFACE_EXTENSION_NAME;
			}
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
			if (!strcmp(VK_MVK_MACOS_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtension(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
			}
#endif

#ifdef USE_EXT_debug_report
			if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				if (enableValidation) {
					AddExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
				}
			}
#else

			if (!strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				if (enableValidation) {
					AddExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				}
			}
#endif
	
		}

		free(instance_extensions);
	}
	

	if (!surfaceExtFound) {
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
	}
	if (!platformSurfaceExtFound) {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_WIN32_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_IOS_MVK)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_MVK_IOS_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_MVK_MACOS_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_XCB_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_MIR_KHR)
#elif defined(VK_USE_PLATFORM_DISPLAY_KHR)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_DISPLAY_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		vkewLogMessage("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_XLIB_SURFACE_EXTENSION_NAME
			" extension.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
#endif
	}
}

static VkDebugUtilsMessengerCreateInfoEXT s_CreateInfoUtilMessenger;
static VkDebugReportCallbackCreateInfoEXT s_CreateInfoDebugReport;


static void ActivateDebugReport()
{	

#ifdef USE_EXT_debug_report
	if (VKEW_EXT_debug_report)
	{
		VkResult err = vkCreateDebugReportCallbackEXT(vkewGetInstance(), &s_CreateInfoDebugReport, 0, &Vulkan.messageCallback);
		switch (err) {
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			vkewLogMessage("CreateDebugUtilsMessengerEXT: out of host memory\n");
			break;
		default:
			vkewLogMessage("CreateDebugUtilsMessengerEXT: unknown failure\n");
			break;
		}
	}
#else

	if (VKEW_EXT_debug_utils)
	{
		VkResult err = vkCreateDebugUtilsMessengerEXT(Vulkan.instance, &s_CreateInfoUtilMessenger, NULL, &Vulkan.debugMessenger);
		switch (err) {
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			vkewLogMessage("CreateDebugUtilsMessengerEXT: out of host memory\n");
			break;
		default:
			vkewLogMessage("CreateDebugUtilsMessengerEXT: unknown failure\n");
			break;
		}
	}
#endif
	else
	{
#ifdef _DEBUG
		vkewLogMessage("No debug report function available!");
#endif
	}
	
}




static void InitDebugReport()
{
	// If requested, we enable the default validation layers for debugging

	if (Vulkan.enableValidation) {
		// Since 1.1, VK_EXT_debug_report is no more available
#ifdef USE_EXT_debug_report
		
		// VK_EXT_debug_report style
		s_CreateInfoDebugReport.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		s_CreateInfoDebugReport.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		s_CreateInfoDebugReport.pfnCallback = debugReportCallback;

		Vulkan.instanceCreateInfo.pNext = &s_CreateInfoDebugReport;
#else
		

		// VK_EXT_debug_utils style
		s_CreateInfoUtilMessenger.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		s_CreateInfoUtilMessenger.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		s_CreateInfoUtilMessenger.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		s_CreateInfoUtilMessenger.pfnUserCallback = debugUtilsCallback;
		Vulkan.instanceCreateInfo.pNext = &s_CreateInfoUtilMessenger;
		
#endif



	}
}


int vkewInit(const char* pApplicationName, const char* pEngineName, int apiVersion, int enableValidation)
{	
	VkResult err;

	const char* enabledExtensions[64];
	const char* validationLayerNames[64];

	Vulkan.validationLayerNames = validationLayerNames;
	Vulkan.validationLayerCount = 0;
	Vulkan.enabledExtensions = enabledExtensions;
	Vulkan.enabledExtensionCount = 0; 

	VkApplicationInfo applicationInfo = { 0 };
	VkInstanceCreateInfo instanceCreateInfo = { 0 };
	Vulkan.enableValidation = enableValidation != 0;

#if defined VK_NO_PROTOTYPES
#if defined(_WIN32)

#ifdef UNICODE
	Vulkan.ctx = LoadLibrary(L"vulkan-1.dll");
#else
	Vulkan.ctx = LoadLibrary("vulkan-1.dll");
#endif
#elif defined(__ANDROID__)
	Vulkan.ctx = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#endif
	if (!Vulkan.ctx) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

#endif


#if defined VK_NO_PROTOTYPES
    // Load base function pointers
	VKEW_GET_PROC(vkEnumerateInstanceExtensionProperties);
	VKEW_GET_PROC(vkEnumerateInstanceLayerProperties);
	VKEW_GET_PROC(vkCreateInstance);
	VKEW_GET_PROC(vkGetInstanceProcAddr);
	VKEW_GET_PROC(vkGetDeviceProcAddr);
#endif

	
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = NULL;
	applicationInfo.pApplicationName = pApplicationName;
	applicationInfo.applicationVersion = 1;
	applicationInfo.pEngineName = pEngineName;
	applicationInfo.engineVersion = 1;
	applicationInfo.apiVersion = apiVersion;

	InitValidationLayers(enableValidation);
	InitExtensionsLayers(enableValidation);
	
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = Vulkan.validationLayerCount;
	instanceCreateInfo.ppEnabledLayerNames = Vulkan.validationLayerNames;
	instanceCreateInfo.enabledExtensionCount = Vulkan.enabledExtensionCount;
	instanceCreateInfo.ppEnabledExtensionNames = Vulkan.enabledExtensions;

	Vulkan.appInfo = applicationInfo;	
	Vulkan.instanceCreateInfo = instanceCreateInfo;

	if (enableValidation) {
		InitDebugReport();
	}
		
	err =  VK_CHECK(vkCreateInstance(&Vulkan.instanceCreateInfo, 
						  NULL,  // If pAllocator is not NULL, pAllocator must be a pointer to a valid VkAllocationCallbacks structure
						  &Vulkan.instance)); //pInstance points a VkInstance handle in which the resulting instance is returned.
	
	if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
		vkewLogMessage(
			"Cannot find a compatible Vulkan installable client driver (ICD).\n\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
	}
	else if (err == VK_ERROR_EXTENSION_NOT_PRESENT) {
		vkewLogMessage(
			"Cannot find a specified extension library.\n"
			"Make sure your layers path is set appropriately.\n",
			"vkCreateInstance Failure");
	}
	else if (err) {
		vkewLogMessage(
			"vkCreateInstance failed.\n\n"
			"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
			"Please look at the Getting Started guide for additional information.\n",
			"vkCreateInstance Failure");
	}
	if (err == VK_SUCCESS)
	{		
		vkewInstanceLevelInit(Vulkan.instance);
		if (enableValidation) {
			ActivateDebugReport();
		}
		
	}

	
	return err;
}

/**
* Acquires the next image in the swap chain
*
* @param presentCompleteSemaphore (Optional) Semaphore that is signaled when the image is ready for use
* @param imageIndex Pointer to the image index that will be increased if the next image could be acquired
*
* @note The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX
*
* @return VkResult of the image acquisition
*/

int vkewGetFrameIndex(void)
{
	return Vulkan.frame_index;
}

VkResult vkewAcquire(uint32_t* current_buffer)
{	
	// DS: vkAcquireNextImageKHR: Semaphore must not be currently signaled or in a wait state.For more information refer to Vulkan Spec Section '30.6. WSI Swapchain' which states 'If semaphore is not VK_NULL_HANDLE it must be unsignaled' (https ://www.khronos.org/registry/vulkan/specs/1.0-extensions/xhtml/vkspec.html#vkAcquireNextImageKHR)
	VkResult ret = VK_CHECK(vkAcquireNextImageKHR(Vulkan.device, 
		Vulkan.swapChainParams.Handle,
		UINT64_MAX,
		Vulkan.presentCompleteSemaphore[Vulkan.frame_index], 
		VK_NULL_HANDLE,
		&Vulkan.current_buffer));


	*current_buffer = Vulkan.current_buffer;
	
	// Use a fence to wait until the command buffer has finished execution before using it again
	if (Vulkan.waitFencesSubmitted[Vulkan.frame_index]) {
		VK_CHECK(vkWaitForFences(Vulkan.device, 1, &Vulkan.waitFences[Vulkan.frame_index], VK_TRUE, UINT64_MAX));
		VK_CHECK(vkResetFences(Vulkan.device, 1, &Vulkan.waitFences[Vulkan.frame_index]));
		Vulkan.waitFencesSubmitted[Vulkan.frame_index] = 0;
	}



	return ret;
}

VkResult vkewQueueSubmit(const VkCommandBuffer* pCommandBuffers, uint32_t commandBufferCount, VkPipelineStageFlags submitPipelineStages, const VkSemaphore* pWaitSemaphores,	const VkSemaphore* pSignalSemaphores)
{

	int useFence = pWaitSemaphores == NULL && pSignalSemaphores == NULL;
	VkFence fence = !useFence ? VK_NULL_HANDLE : Vulkan.waitFences[Vulkan.frame_index];
		
	if (Vulkan.waitFencesSubmitted[Vulkan.frame_index])
	{
		// Fence is already submitted for this frame. 
		return VK_ERROR_DEVICE_LOST;
	}

	VkSubmitInfo submit_info = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
		NULL,                                      // const void                  *pNext
		1,                                            // uint32_t                     waitSemaphoreCount
		pWaitSemaphores ? pWaitSemaphores  : &Vulkan.presentCompleteSemaphore[Vulkan.frame_index],              // const VkSemaphore           *pWaitSemaphores
		&submitPipelineStages,                         // const VkPipelineStageFlags  *pWaitDstStageMask; VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		commandBufferCount,                                            // uint32_t                     commandBufferCount
		pCommandBuffers,  // const VkCommandBuffer       *pCommandBuffers
		1,                                            // uint32_t                     signalSemaphoreCount
		pSignalSemaphores ? pSignalSemaphores : &Vulkan.renderCompleteSemaphore[Vulkan.frame_index]            // const VkSemaphore           *pSignalSemaphores
	};
	


	// Queue 0xbb48f98 is waiting on semaphore 0xe that has no way to be signaled.
	VkResult result = VK_CHECK(vkQueueSubmit(Vulkan.queue, 1, &submit_info, fence));
	if (useFence) {
		Vulkan.waitFencesSubmitted[Vulkan.frame_index] = 1;
	}

	
	return result;
}


VkResult vkewQueuePresent(const VkSemaphore* pWaitSemaphores)
{	
	/*
	  Images passed to present must be in layout VK_IMAGE_LAYOUT_PRESENT_SRC_KHR or VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR but is in VK_IMAGE_LAYOUT_UNDEFINED. The spec valid usage text states 'Each element of pImageIndices must be the index of a presentable image acquired from the swapchain specified by the corresponding element of the pSwapchains array, and the presented image subresource must be in the VK_IMAGE_LAYOUT_PRESENT_SRC_KHR layout at the time the operation is executed on a VkDevice' 
	*/
	VkPresentInfoKHR present_info = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
		NULL,                                      // const void                  *pNext
		1,                                            // uint32_t                     waitSemaphoreCount
		&Vulkan.renderCompleteSemaphore[Vulkan.frame_index],           // const VkSemaphore           *pWaitSemaphores
		1,                                            // uint32_t                     swapchainCount
		&Vulkan.swapChainParams.Handle,                            // const VkSwapchainKHR        *pSwapchains
		&Vulkan.frame_index,                                 // const uint32_t              *pImageIndices
		NULL                                       // VkResult                    *pResults
	};
	VkResult err = VK_CHECK(vkQueuePresentKHR(Vulkan.queue, &present_info));
	
	if (err == VK_ERROR_OUT_OF_DATE_KHR) {
		
	}
	else if (err == VK_SUBOPTIMAL_KHR) {
		
	}
	else {
	
	}
	// Fences has been waited, 
	Vulkan.frame_index += 1;
	Vulkan.frame_index %= Vulkan.swapChainParams.ImagesCount;
	Vulkan.frame_rendered += 1;

	return err;
}

void vkewReleaseSurface(void)
{
	if (Vulkan.device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(Vulkan.device);
		vkDestroyDevice(Vulkan.device, NULL);
		Vulkan.device = VK_NULL_HANDLE;
	}
	for (int i = 0; i < MAX_FENCES; i++) {
		if (Vulkan.presentCompleteSemaphore[i]) {
			vkDestroySemaphore(Vulkan.device, Vulkan.presentCompleteSemaphore[i], NULL);
		}
		if (Vulkan.renderCompleteSemaphore[i]) {
			vkDestroySemaphore(Vulkan.device, Vulkan.renderCompleteSemaphore[i], NULL);
		}
	}
}


void vkewDeviceLevelInit(VkDevice device)
{
#if defined VK_NO_PROTOTYPES
	VK_DEVICE_LEVEL_FUNCTION(vkGetDeviceQueue);
	VK_DEVICE_LEVEL_FUNCTION(vkDeviceWaitIdle);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyDevice);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateSemaphore);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateCommandPool);
	VK_DEVICE_LEVEL_FUNCTION(vkAllocateCommandBuffers);
	VK_DEVICE_LEVEL_FUNCTION(vkBeginCommandBuffer);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdPipelineBarrier);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdClearColorImage);
	VK_DEVICE_LEVEL_FUNCTION(vkEndCommandBuffer);
	VK_DEVICE_LEVEL_FUNCTION(vkQueueSubmit);
	VK_DEVICE_LEVEL_FUNCTION(vkFreeCommandBuffers);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyCommandPool);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroySemaphore);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateImageView);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateRenderPass);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateFramebuffer);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateShaderModule);
	VK_DEVICE_LEVEL_FUNCTION(vkCreatePipelineLayout);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateGraphicsPipelines);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdBeginRenderPass);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdBindPipeline);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdDraw);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdEndRenderPass);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyShaderModule);;
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyPipelineLayout);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyPipeline);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyRenderPass);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyFramebuffer);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyImageView);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateFence);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateBuffer);
	VK_DEVICE_LEVEL_FUNCTION(vkGetBufferMemoryRequirements);
	VK_DEVICE_LEVEL_FUNCTION(vkAllocateMemory);
	VK_DEVICE_LEVEL_FUNCTION(vkBindBufferMemory);
	VK_DEVICE_LEVEL_FUNCTION(vkMapMemory);
	VK_DEVICE_LEVEL_FUNCTION(vkFlushMappedMemoryRanges);
	VK_DEVICE_LEVEL_FUNCTION(vkUnmapMemory);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdSetViewport);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdSetScissor);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdBindVertexBuffers);
	VK_DEVICE_LEVEL_FUNCTION(vkWaitForFences);
	VK_DEVICE_LEVEL_FUNCTION(vkResetFences);
	VK_DEVICE_LEVEL_FUNCTION(vkFreeMemory);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyBuffer);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyFence);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdCopyBuffer);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateImage);
	VK_DEVICE_LEVEL_FUNCTION(vkGetImageMemoryRequirements);
	VK_DEVICE_LEVEL_FUNCTION(vkBindImageMemory);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateSampler);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdCopyBufferToImage);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateDescriptorSetLayout);
	VK_DEVICE_LEVEL_FUNCTION(vkCreateDescriptorPool);
	VK_DEVICE_LEVEL_FUNCTION(vkAllocateDescriptorSets);
	VK_DEVICE_LEVEL_FUNCTION(vkUpdateDescriptorSets);
	VK_DEVICE_LEVEL_FUNCTION(vkCmdBindDescriptorSets);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyDescriptorPool);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyDescriptorSetLayout);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroySampler);
	VK_DEVICE_LEVEL_FUNCTION(vkDestroyImage);
#endif



#ifdef VK_KHR_android_surface
    vkewInit_VK_KHR_android_surface(device);
#endif /* VK_USE_PLATFORM_ANDROID_KHR */


#ifdef VK_KHR_xlib_surface

	VK_DEVICE_LEVEL_FUNCTION(vkCreateXlibSurfaceKHR);
	VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceXlibPresentationSupportKHR);


#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_KHR_xcb_surface

	VK_DEVICE_LEVEL_FUNCTION(vkCreateXcbSurfaceKHR);
	VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceXcbPresentationSupportKHR);


#endif /* VK_USE_PLATFORM_XCB_KHR */	


#ifdef VK_KHR_wayland_surface

	VK_DEVICE_LEVEL_FUNCTION(vkCreateWaylandSurfaceKHR);
	VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceWaylandPresentationSupportKHR);

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_KHR_mir_surface

	VK_DEVICE_LEVEL_FUNCTION(vkCreateMirSurfaceKHR);
	VK_DEVICE_LEVEL_FUNCTION(vkGetPhysicalDeviceMirPresentationSupportKHR);

#endif /* VK_USE_PLATFORM_MIR_KHR */



#endif
}


VkBool32 vkewCheckPhysicalDeviceProperties(VkPhysicalDevice physical_device, uint32_t *selected_graphics_queue_family_index, uint32_t *selected_present_queue_family_index) {
	uint32_t extensions_count = 0;

	vkGetPhysicalDeviceProperties(physical_device, &Vulkan.deviceProperties);
	vkGetPhysicalDeviceFeatures(physical_device, &Vulkan.deviceFeatures);

	if ((vkEnumerateDeviceExtensionProperties(physical_device, NULL, &extensions_count, NULL) != VK_SUCCESS) ||
		(extensions_count == 0)) {
		return VK_FALSE;
	}

	VkExtensionProperties* available_extensions = (VkExtensionProperties*)calloc(extensions_count, sizeof(VkExtensionProperties));

	if (vkEnumerateDeviceExtensionProperties(physical_device, NULL, &extensions_count, &available_extensions[0]) != VK_SUCCESS) {
		return VK_FALSE;
	}

	int device_extensions_count = 1;
	const char* device_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	for (int i = 0; i < device_extensions_count; ++i) {
		if (!_vkewCheckExtensionAvailability(device_extensions[i], available_extensions, extensions_count)) {
			return VK_FALSE;
		}
	}




	uint32_t major_version = VK_VERSION_MAJOR(Vulkan.deviceProperties.apiVersion);

	if ((major_version < 1) ||
		(Vulkan.deviceProperties.limits.maxImageDimension2D < 4096)) {
		return VK_FALSE;
	}

	uint32_t queue_families_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, NULL);
	if (queue_families_count == 0) {
		return VK_FALSE;
	}

	VkQueueFamilyProperties* queue_family_properties = (VkQueueFamilyProperties*)calloc(queue_families_count, sizeof(VkQueueFamilyProperties));
	VkBool32* queue_present_support = (VkBool32*)calloc(queue_families_count, sizeof(VkBool32));

	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, &queue_family_properties[0]);

	uint32_t graphics_queue_family_index = UINT32_MAX;
	uint32_t present_queue_family_index = UINT32_MAX;

	for (uint32_t i = 0; i < queue_families_count; ++i) {
		vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, Vulkan.presentationSurface, &queue_present_support[i]);

		if ((queue_family_properties[i].queueCount > 0) &&
			(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
			// Select first queue that supports graphics
			if (graphics_queue_family_index == UINT32_MAX) {
				graphics_queue_family_index = i;
			}

			// If there is queue that supports both graphics and present - prefer it
			if (queue_present_support[i]) {
				*selected_graphics_queue_family_index = i;
				*selected_present_queue_family_index = i;
				return VK_TRUE;
			}
		}
	}

	// We don't have queue that supports both graphics and present so we have to use separate queues
	for (uint32_t i = 0; i < queue_families_count; ++i) {
		if (queue_present_support[i]) {
			present_queue_family_index = i;
			break;
		}
	}

	// If this device doesn't support queues with graphics and present capabilities don't use it
	if ((graphics_queue_family_index == UINT32_MAX) ||
		(present_queue_family_index == UINT32_MAX)) {
		return VK_FALSE;
	}

	*selected_graphics_queue_family_index = graphics_queue_family_index;
	*selected_present_queue_family_index = present_queue_family_index;
	return VK_TRUE;
}


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkFlags msgFlags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject,
	size_t location,
	int32_t msgCode,
	const char *pLayerPrefix,
	const char *pMsg,
	void *pUserData)
{
	(void)msgFlags; (void)objType; (void)srcObject; (void)location; (void)pUserData; (void)msgCode;
	vkewLogMessage("%s: %s\n", pLayerPrefix, pMsg);
	// If the application returns VK_TRUE from its callback and the Vulkan call being aborted returns a VkResult, the layer will return VK_ERROR_VALIDATION_FAILED_EXT.
	return msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT ? VK_TRUE : VK_FALSE;
}

VkResult vkewCreateDevice()
{	
	VkInstance vkInstance = vkewGetInstance();
	uint32_t num_devices;

	if ((VK_CHECK(vkEnumeratePhysicalDevices(vkInstance, &num_devices, NULL)) != VK_SUCCESS) ||
		(num_devices == 0)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkPhysicalDevice* physical_devices = (VkPhysicalDevice*)calloc(num_devices, sizeof(VkPhysicalDevice));
	if (VK_CHECK(vkEnumeratePhysicalDevices(vkInstance, &num_devices, &physical_devices[0])) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	uint32_t selected_graphics_queue_family_index = UINT32_MAX;
	uint32_t selected_present_queue_family_index = UINT32_MAX;

	for (uint32_t i = 0; i < num_devices; ++i) {
		if (vkewCheckPhysicalDeviceProperties(physical_devices[i], &selected_graphics_queue_family_index, &selected_present_queue_family_index)) {
			Vulkan.physicalDevice = physical_devices[i];
		}
	}
	if (Vulkan.physicalDevice == VK_NULL_HANDLE) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkDeviceQueueCreateInfo queue_create_infos[2];
	float queue_priorities[] = { 1.0f };
	int queueCount = 1;

	VkDeviceQueueCreateInfo qci0 ={
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType              sType
		NULL,                                          // const void                  *pNext
		0,                                                // VkDeviceQueueCreateFlags     flags
		selected_graphics_queue_family_index,             // uint32_t                     queueFamilyIndex
		queueCount,   // uint32_t                     queueCount
		&queue_priorities[0]                              // const float                 *pQueuePriorities
	};

	int queueCreateInfoCount = 0;

	VkDeviceQueueCreateInfo qci1 = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
		NULL,                                        // const void                  *pNext
		0,                                              // VkDeviceQueueCreateFlags     flags
		selected_present_queue_family_index,            // uint32_t                     queueFamilyIndex
		queueCount, // uint32_t                     queueCount
		&queue_priorities[0]                            // const float                 *pQueuePriorities
	};

	queue_create_infos[0] = qci0;
	queueCreateInfoCount++;

	if (selected_graphics_queue_family_index != selected_present_queue_family_index) {
		queue_create_infos[1] = qci1;
		queueCreateInfoCount++;
	}

	int enabledExtensionCount = 1;
	const char *ppEnabledExtensionNames[64] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	
	VkDeviceCreateInfo device_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,             // VkStructureType                    sType
		NULL,                                          // const void                        *pNext
		0,                                                // VkDeviceCreateFlags                flags
		queueCreateInfoCount, // uint32_t                           queueCreateInfoCount
		&queue_create_infos[0],                           // const VkDeviceQueueCreateInfo     *pQueueCreateInfos
		0,                                                // uint32_t                           enabledLayerCount
		NULL,                                          // const char * const                *ppEnabledLayerNames
		enabledExtensionCount,         // uint32_t                           enabledExtensionCount
		ppEnabledExtensionNames,                                   // const char * const                *ppEnabledExtensionNames
		NULL                                           // const VkPhysicalDeviceFeatures    *pEnabledFeatures
	};

	if (VK_CHECK(vkCreateDevice(Vulkan.physicalDevice, &device_create_info, NULL, &Vulkan.device)) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	if (Vulkan.device == VK_NULL_HANDLE) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	Vulkan.graphicsQueueFamilyIndex = selected_graphics_queue_family_index;
	Vulkan.presentQueueFamilyIndex = selected_present_queue_family_index;
	Vulkan.queueNodeIndex = Vulkan.graphicsQueueFamilyIndex;

	vkGetDeviceQueue(Vulkan.device, Vulkan.queueNodeIndex, 0, &Vulkan.queue);
	
	// Create a semaphore used to synchronize image presentation
	// Ensures that the image is displayed before we start submitting new commands to the queu

	VkSemaphoreCreateInfo semaphoreCreateInfo = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,       // VkStructureType          sType
		NULL,							               // const void*              pNext
		0									          // VkSemaphoreCreateFlags   flags
	};

	// Fences (Used to check draw command buffer completion)
	VkFenceCreateInfo fence_ci = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		NULL,
		0
	};

	for (uint32_t i = 0; i < MAX_FENCES; i++) {

		VK_CHECK(vkCreateFence(Vulkan.device, &fence_ci, NULL, Vulkan.waitFences + i));

		if ((VK_CHECK(vkCreateSemaphore(Vulkan.device, &semaphoreCreateInfo, NULL, &Vulkan.presentCompleteSemaphore[i])) != VK_SUCCESS) ||
			(VK_CHECK(vkCreateSemaphore(Vulkan.device, &semaphoreCreateInfo, NULL, &Vulkan.renderCompleteSemaphore[i])) != VK_SUCCESS)) {
			return VK_ERROR_LAYER_NOT_PRESENT;
		}
		if (Vulkan.separatePresentQueue) {
			VK_CHECK(vkCreateSemaphore(Vulkan.device, &semaphoreCreateInfo, NULL, &Vulkan.imageOwnershipSemaphores[i]));
		}

		
	}
	
	
	vkGetPhysicalDeviceProperties(Vulkan.physicalDevice, &Vulkan.deviceProperties);
	vkGetPhysicalDeviceFeatures(Vulkan.physicalDevice, &Vulkan.deviceFeatures);
	return VK_SUCCESS;
}

void vkewGetPropertiesAndFeatures(VkPhysicalDeviceProperties* p, VkPhysicalDeviceFeatures* f)
{
	*p = Vulkan.deviceProperties;
	*f = Vulkan.deviceFeatures;
}

VkResult vkewCreateSurface(int deviceIndex, void* platformHandle, void * platformWindow)
{

	VkResult result;
	

#if defined(_WIN32)
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)platformHandle; // provided by the platform code
	surfaceCreateInfo.hwnd = (HWND)platformWindow;           // provided by the platform code
	result = VK_CHECK(vkCreateWin32SurfaceKHR(Vulkan.instance, &surfaceCreateInfo, NULL, &Vulkan.presentationSurface));


	
#elif defined(__ANDROID__)
	VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.window = platformWindow;                       // provided by the platform code
	result = vkCreateAndroidSurfaceKHR(vkewGetInstance(), &surfaceCreateInfo, NULL, &Vulkan.presentationSurface);
#else
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.connection = platformHandle;               // provided by the platform code
	surfaceCreateInfo.window = platformWindow;                       // provided by the platform code
	VkResult result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface);
#endif

	if (result != VK_SUCCESS) {
		return result;
	}
	

	result = VK_CHECK(vkewCreateDevice());
	if (result != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	vkewDeviceLevelInit(Vulkan.device);

	
	vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &Vulkan.formatCount, NULL);
	Vulkan.surfaceFormats = (VkSurfaceFormatKHR*)calloc(Vulkan.formatCount, sizeof(VkSurfaceFormatKHR));
	vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &Vulkan.formatCount, &Vulkan.surfaceFormats[0]);

	// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
	// the surface has no preferred format. Otherwise, at least one
	// supported format will be returned
	if (Vulkan.formatCount == 1 && Vulkan.surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
		Vulkan.colorFormat = DEFAULT_FRAME_BUFFER_FORMAT;
	else {
		Vulkan.colorFormat = Vulkan.surfaceFormats[0].format;
	}
	Vulkan.colorSpace = Vulkan.surfaceFormats[0].colorSpace;
	


	return VK_SUCCESS;
}

VkFormat vkewGetColorFormat()
{
	return Vulkan.colorFormat;
}


uint32_t vkewGetSwapChainNumImages(const VkSurfaceCapabilitiesKHR* surface_capabilities) {
	// Set of images defined in a swap chain may not always be available for application to render to:
	// One may be displayed and one may wait in a queue to be presented
	// If application wants to use more images at the same time it must ask for more images
	uint32_t image_count = surface_capabilities->minImageCount + 1;
	if ((surface_capabilities->maxImageCount > 0) &&
		(image_count > surface_capabilities->maxImageCount)) {
		image_count = surface_capabilities->maxImageCount;
	}
	return image_count;
}



VkSurfaceFormatKHR vkewGetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count, int hdr) 
{
	// If the list contains only one entry with undefined format
	// it means that there are no preferred surface formats and any can be chosen
	int i;
	if ((count == 1) &&
		(surface_formats[0].format == VK_FORMAT_UNDEFINED)) {
		VkSurfaceFormatKHR ret;
		ret.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		ret.format = DEFAULT_FRAME_BUFFER_FORMAT;
		return ret;
	}

	// Check if list contains most widely used R8 G8 B8 A8 format
	// with nonlinear color space
	for (i = 0; i < count; i++) {
		if (surface_formats[i].format == DEFAULT_FRAME_BUFFER_FORMAT) {
			return surface_formats[i];
		}
	}

	// Return the first format from the list
	return surface_formats[0];
}

VkExtent2D vkewGetSwapChainExtent(const VkSurfaceCapabilitiesKHR* surface_capabilities) {
	// Special value of surface extent is width == height == -1
	// If this is so we define the size by ourselves but it must fit within defined confines
	if (surface_capabilities->currentExtent.width == -1) {
		VkExtent2D swap_chain_extent = { 640, 480 };
		if (swap_chain_extent.width < surface_capabilities->minImageExtent.width) {
			swap_chain_extent.width = surface_capabilities->minImageExtent.width;
		}
		if (swap_chain_extent.height < surface_capabilities->minImageExtent.height) {
			swap_chain_extent.height = surface_capabilities->minImageExtent.height;
		}
		if (swap_chain_extent.width > surface_capabilities->maxImageExtent.width) {
			swap_chain_extent.width = surface_capabilities->maxImageExtent.width;
		}
		if (swap_chain_extent.height > surface_capabilities->maxImageExtent.height) {
			swap_chain_extent.height = surface_capabilities->maxImageExtent.height;
		}
		return swap_chain_extent;
	}

	// Most of the cases we define size of the swap_chain images equal to current window's size
	return surface_capabilities->currentExtent;
}

VkImageUsageFlags vkewGetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR* surface_capabilities) {
	// Color attachment flag must always be supported
	// We can define other usage flags but we always need to check if they are supported
	if (surface_capabilities->supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
		return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}
	return (VkImageUsageFlags)(-1);
}

VkSurfaceTransformFlagBitsKHR vkewGetSwapChainTransform(const VkSurfaceCapabilitiesKHR* surface_capabilities) {
	// Sometimes images must be transformed before they are presented (i.e. due to device's orienation
	// being other than default orientation)
	// If the specified transform is other than current transform, presentation engine will transform image
	// during presentation operation; this operation may hit performance on some platforms
	// Here we don't want any transformations to occur so if the identity transform is supported use it
	// otherwise just use the same transform as current transform
	if (surface_capabilities->supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else {
		return surface_capabilities->currentTransform;
	}
}

VkPresentModeKHR vkewGetSwapChainPresentMode(const VkPresentModeKHR* present_modes, int count, int vsync) {
	// FIFO present mode is always available
	// MAILBOX is the lowest latency -Sync enabled mode (something like triple-buffering) so use it if available
	int i;

	// Check for VK_PRESENT_MODE_IMMEDIATE_KHR
	if (vsync == 0)
	{
		for (i = 0; i < count; i++)
			if (VK_PRESENT_MODE_IMMEDIATE_KHR == present_modes[i])
				return present_modes[i];
	}


	// Very bad for mobile
	for (i = 0; i < count; i++)
		if (VK_PRESENT_MODE_MAILBOX_KHR == present_modes[i])
			return present_modes[i];


	for (i = 0; i < count; i++)
		if (VK_PRESENT_MODE_FIFO_KHR == present_modes[i])
			return present_modes[i];

	
	return (VkPresentModeKHR)(-1);
}


void vkewReleaseSwapChain()
{
	vkDestroySwapchainKHR(Vulkan.device, Vulkan.swapChainParams.Handle, NULL);
	Vulkan.swapChainParams.Handle = VK_NULL_HANDLE;
	
}

VkResult vkewCreateCommandPool()
{
	VkCommandPoolCreateInfo cmd_pool_create_info = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,     // VkStructureType              sType
		NULL,                                        // const void*                  pNext
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,       // VkCommandPoolCreateFlags     flags
		Vulkan.queueNodeIndex                  // uint32_t                     queueFamilyIndex
	};

	return vkCreateCommandPool(Vulkan.device, &cmd_pool_create_info, NULL, &Vulkan.presentQueueCmdPool);
}


VkResult vkewCreateCommandBuffers()
{

	VkResult ret;
	
	ret = vkewCreateCommandPool();
	if (VK_CHECK(ret) != VK_SUCCESS)
		return ret;

	if (Vulkan.swapChainParams.Handle == VK_NULL_HANDLE) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	uint32_t commandBufferCount = 0;
	if (VK_CHECK(vkGetSwapchainImagesKHR(Vulkan.device, Vulkan.swapChainParams.Handle, &commandBufferCount, NULL) != VK_SUCCESS) ||
		(commandBufferCount == 0)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}


	Vulkan.presentQueueCmdBuffers = (VkCommandBuffer*) realloc(Vulkan.presentQueueCmdBuffers, sizeof(VkCommandBuffer) * commandBufferCount);
	Vulkan.presentQueueCmdBuffersCount = commandBufferCount;


	VkCommandBufferAllocateInfo cmd_buffer_allocate_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // VkStructureType              sType
		NULL,                                        // const void*                  pNext
		Vulkan.presentQueueCmdPool,                     // VkCommandPool                commandPool
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // VkCommandBufferLevel         level
		commandBufferCount                                     // uint32_t                     bufferCount
	};
	return VK_CHECK(vkAllocateCommandBuffers(Vulkan.device, &cmd_buffer_allocate_info, &Vulkan.presentQueueCmdBuffers[0]));
}

VkSwapchainKHR vkewGetSwapChain(void)
{
	return Vulkan.swapChainParams.Handle;
}


VkExtent2D vkewGetSwapChainExtent2D(void)
{
	return Vulkan.swapChainParams.Extent;
}

VkCommandPool vkewGetPresentQueuePool(void)
{
	return Vulkan.presentQueueCmdPool;
}

VkQueue vkewGetGraphicsQueue(void)
{
	return Vulkan.queue;
}

int vkewGetGraphicsQueueFamilyIndex(void)
{
	return Vulkan.graphicsQueueFamilyIndex;
}

VkDevice vkewGetDevice(void)
{
	return Vulkan.device;
}


VkBool32 vkewCreateSwapChainImageViews(void) {
	
	for (int i = 0; i < Vulkan.swapChainParams.ImagesCount; ++i) {
		VkImageViewCreateInfo image_view_create_info = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   // VkStructureType                sType
			NULL,                                    // const void                    *pNext
			0,                                          // VkImageViewCreateFlags         flags
			Vulkan.swapChainParams.Images[i].Handle,          // VkImage                        image
			VK_IMAGE_VIEW_TYPE_2D,                      // VkImageViewType                viewType
			Vulkan.swapChainParams.Format,                      // VkFormat                       format
			{                                           // VkComponentMapping             components
				VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             r
				VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             g
				VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             b
				VK_COMPONENT_SWIZZLE_IDENTITY               // VkComponentSwizzle             a
			},
			{                                           // VkImageSubresourceRange        subresourceRange
				VK_IMAGE_ASPECT_COLOR_BIT,                  // VkImageAspectFlags             aspectMask
				0,                                          // uint32_t                       baseMipLevel
			1,                                          // uint32_t                       levelCount
			0,                                          // uint32_t                       baseArrayLayer
			1                                           // uint32_t                       layerCount
			}
		};

		if (vkCreateImageView(Vulkan.device, &image_view_create_info, NULL, &Vulkan.swapChainParams.Images[i].View) != VK_SUCCESS) {
			return VK_FALSE;
		}
	}

	return VK_TRUE;
}


VkResult vkewClearCommand()
{	
	if (Vulkan.device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(Vulkan.device);

		if ((Vulkan.presentQueueCmdBuffersCount) && (Vulkan.presentQueueCmdBuffers[0] != VK_NULL_HANDLE)) {
			vkFreeCommandBuffers(Vulkan.device, Vulkan.presentQueueCmdPool, Vulkan.presentQueueCmdBuffersCount, &Vulkan.presentQueueCmdBuffers[0]);
			Vulkan.presentQueueCmdBuffersCount = 0;
		}

		if (Vulkan.presentQueueCmdPool != VK_NULL_HANDLE) {
			vkDestroyCommandPool(Vulkan.device, Vulkan.presentQueueCmdPool, NULL);
			Vulkan.presentQueueCmdPool = VK_NULL_HANDLE;
		}
	}
	return VK_SUCCESS;
}

// Create swap chain
// @param : vsync : enable vertical sync, if false, will try to use VK_PRESENT_MODE_IMMEDIATE_KHR
// @param : exclusive : enable exclusive mode (TODO: Not implemented, see VK_KHR_display_swapchain).
// https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/master/appendices/VK_KHR_display_swapchain.txt

VkResult vkewCreateSwapChain(void * platformWindow, int vsync, int full_screen_exclusive, int hdr)
{
	
	if (Vulkan.device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(Vulkan.device);
	}

	VkSurfaceCapabilitiesKHR surface_capabilities;
	if (VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &surface_capabilities)) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	uint32_t formats_count;
	if ((VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &formats_count, NULL)) != VK_SUCCESS) ||
		(formats_count == 0)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkSurfaceFormatKHR* surface_formats = (VkSurfaceFormatKHR*)calloc(formats_count, sizeof(VkSurfaceFormatKHR));
	if (VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &formats_count, &surface_formats[0])) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	uint32_t present_modes_count;
	if ((VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &present_modes_count, NULL)) != VK_SUCCESS) ||
		(present_modes_count == 0)) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkPresentModeKHR* present_modes = (VkPresentModeKHR*)calloc(present_modes_count, sizeof(VkSurfaceFormatKHR));
	if (VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &present_modes_count, &present_modes[0])) != VK_SUCCESS) {
		return VK_ERROR_FORMAT_NOT_SUPPORTED;
	}

	uint32_t                      desired_number_of_images = vkewGetSwapChainNumImages(&surface_capabilities);
	VkSurfaceFormatKHR            desired_format = vkewGetSwapChainFormat(surface_formats, formats_count, hdr);
	VkExtent2D                    desired_extent = vkewGetSwapChainExtent(&surface_capabilities);
	VkImageUsageFlags             desired_usage = vkewGetSwapChainUsageFlags(&surface_capabilities);
	VkSurfaceTransformFlagBitsKHR desired_transform = vkewGetSwapChainTransform(&surface_capabilities);
	VkPresentModeKHR              desired_present_mode = vkewGetSwapChainPresentMode(present_modes, present_modes_count, vsync);
	VkSwapchainKHR                old_swap_chain = Vulkan.swapChainParams.Handle;

	if ((int)(desired_usage) == -1) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	if ((int)(desired_present_mode) == -1) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	if ((desired_extent.width == 0) || (desired_extent.height == 0)) {
		// Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
		// But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
		return VK_SUCCESS;
	}

	VkSurfaceTransformFlagsKHR preTransform;
	if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else {
		preTransform = surface_capabilities.currentTransform;
	}

	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};
	for (uint32_t i = 0; i < ARRAY_SIZE(compositeAlphaFlags); i++) {
		if (surface_capabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
			compositeAlpha = compositeAlphaFlags[i];
			break;
		}
	}

	VkSwapchainCreateInfoKHR swap_chain_create_info = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // VkStructureType                sType
		NULL,										   // const void                    *pNext
		0,                                            // VkSwapchainCreateFlagsKHR      flags
		Vulkan.presentationSurface,                   // VkSurfaceKHR                   surface
		desired_number_of_images,                     // uint32_t                       minImageCount
		desired_format.format,                        // VkFormat                       imageFormat
		desired_format.colorSpace,                    // VkColorSpaceKHR                imageColorSpace
		desired_extent,                               // VkExtent2D                     imageExtent
		1,                                            // uint32_t                       imageArrayLayers
		desired_usage,                                // VkImageUsageFlags              imageUsage
		VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode                  imageSharingMode
		0,                                            // uint32_t                       queueFamilyIndexCount
		NULL,										 // const uint32_t                *pQueueFamilyIndices
		desired_transform,                            // VkSurfaceTransformFlagBitsKHR  preTransform
		compositeAlpha,								  // VkCompositeAlphaFlagBitsKHR    compositeAlpha
		desired_present_mode,                         // VkPresentModeKHR               presentMode
		VK_TRUE,                                      // VkBool32                       clipped
		old_swap_chain                                // VkSwapchainKHR                 oldSwapchain
	};


#ifdef VK_EXT_full_screen_exclusive

	// https://gpuopen.com/using-amd-freesync-premium-pro-hdr-code-samples/

	VkSurfaceFullScreenExclusiveWin32InfoEXT s_SurfaceFullScreenExclusiveWin32InfoEXT = {
		VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT,
		NULL,
		MonitorFromWindow((HWND)platformWindow, MONITOR_DEFAULTTOPRIMARY)
	};

	VkSurfaceFullScreenExclusiveInfoEXT s_SurfaceFullScreenExclusiveInfoEXT = {
		VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT,
		&s_SurfaceFullScreenExclusiveWin32InfoEXT,
		VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT
	};

	VkPhysicalDeviceSurfaceInfo2KHR s_PhysicalDeviceSurfaceInfo2KHR = {
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
		&s_SurfaceFullScreenExclusiveInfoEXT,
		Vulkan.presentationSurface
	};


	if (full_screen_exclusive && VKEW_EXT_full_screen_exclusive)
	{
		swap_chain_create_info.pNext = &s_PhysicalDeviceSurfaceInfo2KHR;
	}

#endif

	if (VK_CHECK(vkCreateSwapchainKHR(Vulkan.device, &swap_chain_create_info, NULL, &Vulkan.swapChainParams.Handle)) != VK_SUCCESS) {
				return VK_ERROR_INITIALIZATION_FAILED;
	}
	if (old_swap_chain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(Vulkan.device, old_swap_chain, NULL);
	}


    Vulkan.swapChainParams.Format = desired_format.format;

    uint32_t image_count = 0;
    if( (vkGetSwapchainImagesKHR( Vulkan.device, Vulkan.swapChainParams.Handle, &image_count, NULL ) != VK_SUCCESS) ||
        (image_count == 0) ) {
      return VK_FALSE;
    }


	Vulkan.swapChainParams.ImagesCount = image_count; 
	Vulkan.swapChainParams.Images = realloc(Vulkan.swapChainParams.Images, sizeof(struct ImageParameters) * image_count);
	memset(Vulkan.swapChainParams.Images, 0, sizeof(struct ImageParameters) * image_count);

	VkImage* images = (VkImage*) calloc(image_count, sizeof(VkImage) );
    if( vkGetSwapchainImagesKHR( Vulkan.device, Vulkan.swapChainParams.Handle, &image_count, &images[0] ) != VK_SUCCESS ) {
      return VK_FALSE;
    }

    for( int i = 0; i <Vulkan.swapChainParams.ImagesCount; ++i ) {
      Vulkan.swapChainParams.Images[i].Handle = images[i];
    }

    Vulkan.swapChainParams.Extent = desired_extent;
	vkewCreateSwapChainImageViews();

	return VK_SUCCESS;
	
}

uint32_t vkewGetPresentQueueCmdBuffers()
{
	return Vulkan.presentQueueCmdBuffersCount;
}


VkPhysicalDevice vkewGetPhysicalDevice()
{
	return Vulkan.physicalDevice;
}



VkResult vkewInitDescriptorPool(uint32_t descriptorSetCount, VkDescriptorPool* Pool, int maxSets)
{	
	VkDescriptorPoolSize poolSizes[2];
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[0].descriptorCount = 256;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 256;

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,  // VkStructureType                sType
		NULL,											// const void                    *pNext
		0,                                              // VkDescriptorPoolCreateFlags    flags
		maxSets,		                                        // uint32_t                       maxSets
		2,                                              // uint32_t                       poolSizeCount
		poolSizes                                       // const VkDescriptorPoolSize    *pPoolSizes
	};

	return VK_CHECK(vkCreateDescriptorPool(vkewGetDevice(), &descriptor_pool_create_info, NULL, Pool));
}



VkImageView vkewGetSwapChainImageView(int i)
{
	return Vulkan.swapChainParams.Images[i].View;
}

// Vkew Pipeline Builder


// The function we're going to write now involves recording and executing a command buffer again, so now's a good time to move that logic into a helper function or two:

VkCommandBuffer vkewBeginSingleTimeCommands() {
	VkCommandBufferAllocateInfo allocInfo = {0};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = Vulkan.presentQueueCmdPool;
	if (!allocInfo.commandPool)
		return VK_NULL_HANDLE;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(vkewGetDevice(), &allocInfo, &commandBuffer);

	

	return commandBuffer;
}

void vkewEndSingleTimeCommands(VkCommandBuffer commandBuffer) {
	

	VkSubmitInfo submitInfo[1] = {0};
	submitInfo[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo[0].commandBufferCount = 1;
	submitInfo[0].pCommandBuffers = &commandBuffer;

	
	/*/
	
	submit_info[0].waitSemaphoreCount = 1;
	submit_info[0].pWaitSemaphores = &imageAcquiredSemaphore;
	submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
	
	*/


	VK_CHECK(vkQueueSubmit(Vulkan.queue, 1, submitInfo, VK_NULL_HANDLE));
	VK_CHECK(vkQueueWaitIdle(Vulkan.queue));

	vkFreeCommandBuffers(vkewGetDevice(), Vulkan.presentQueueCmdPool, 1, &commandBuffer);
}

void vkewEndSingleTimeCommandsWithSemaphores(VkCommandBuffer commandBuffer, int waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, const VkPipelineStageFlags* pWaitDstStageMask) 
{

	VkSubmitInfo submitInfo[1] = { 0 };
	submitInfo[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo[0].commandBufferCount = 1;
	submitInfo[0].pCommandBuffers = &commandBuffer;

	submitInfo[0].waitSemaphoreCount = waitSemaphoreCount;
	submitInfo[0].pWaitSemaphores = pWaitSemaphores;
	submitInfo[0].pWaitDstStageMask = pWaitDstStageMask;
	

	VK_CHECK(vkQueueSubmit(Vulkan.queue, 1, submitInfo, VK_NULL_HANDLE));
	VK_CHECK(vkQueueWaitIdle(Vulkan.queue));

	vkFreeCommandBuffers(vkewGetDevice(), Vulkan.presentQueueCmdPool, 1, &commandBuffer);
}