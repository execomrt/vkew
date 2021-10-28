/*
 ** The Vulkan Extension Wrangler Library
 ** Copyright (C) 2021 Stephane Denis
 ** All rights reserved.
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
  */
#define VK_NO_PROTOTYPES 1

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#undef UNIX
#define UNIX 1
#endif
#if defined(linux) || defined(__linux) || defined(__linux__) || defined __ANDROID__
#undef UNIX 
#define UNIX 1
#endif

#ifdef WIN32
  /* Windows */
#include <Windows.h>
#define VK_USE_PLATFORM_WIN32_KHR 1
#define VK_USE_PLATFORM_DISPLAY_KHR 1
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define l_strncat(dest, destsz, src) strncat_s(dest, destsz, src, destsz)
#define l_snprintf snprintf
#elif defined UNIX
  /* Android */
#include <stddef.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __ANDROID__
#include <android/log.h>
#define VK_USE_PLATFORM_ANDROID_KHR
#else
#define VK_USE_PLATFORM_XCB_KHR

#endif
#define l_strncat(dest, destsz, src) strncat(dest, src, destsz-1)
#define l_snprintf snprintf
#else
#error "Unsupported platform"
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
  // https://harrylovescode.gitbooks.io/vulkan-api/content/chap04/chap04-linux.html
#include <xcb/xcb.h>
#endif

#include "vkew.h"

#define APP_SHORT_NAME "vkew"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

  // This is incompatible with Render Doc
  // #define USE_EXT_debug_report
  // Default frame buffer format

typedef struct VkInterface
{
	VkInstance instance;
	VkDevice device;
}VkInterface;

/*
   vkGetInstanceProcAddress is to get the function pointer that will always work with any device created from the instance passed in.
   However the functions returned may include dispatch logic(typically to account for extensions that may or may not be enabled for the device)
   that may slow down the call.

   This is why the vkGetDeviceProcAddress exist to get the function that doesn't have dispatch logic.

   You are not obliged to use them but it may help get some extra speed.
*/

static PFN_vkVoidFunction vkewGetInstanceProcAddr(VkInstance instance, const char* name)
{
	return vkGetInstanceProcAddr(instance, name);
}
static PFN_vkVoidFunction vkewGetDeviceProcAddr(VkDevice device, const char* name)
{
	return vkGetDeviceProcAddr(device, name);
}

static PFN_vkVoidFunction vkewGetInterfaceProcAddr(VkInterface value, const char* name)
{
	void* ret = value.device ? vkewGetDeviceProcAddr(value.device, name) : vkGetInstanceProcAddr(value.instance, name);
	if (ret == NULL && value.device != VK_NULL_HANDLE)
	{
		//	vkewLogMessage("<INFO> Entry point not found %s\n", name);
		return vkGetInstanceProcAddr(value.instance, name);
	}
	else
	{
		return ret;
	}
}


#if 1 
static inline const char* vkObjectType_ToString(VkObjectType input_value)
{
	switch ((VkObjectType)input_value)
	{
	case VK_OBJECT_TYPE_QUERY_POOL:
		return "VK_OBJECT_TYPE_QUERY_POOL";
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
	case VK_RESULT_MAX_ENUM:
		return "VK_RESULT_MAX_ENUM";
	case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
		return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
	default:
	{
		static char unknown[32];
		l_snprintf(unknown, 32, "<unknown> %d", result);
		return unknown;
	}
	}
}

VkResult vkewPrintResult(VkResult result, const char* file, int line) {
	if (result != VK_SUCCESS) {
		vkewLogMessage("%s(%d): %s", file, line, VkResult_ToString(result));
	}
	return result;
}
#else

VkResult vkewPrintResult(VkResult result, const char* file, int line)
{
	return result;
}
#endif


/// <summary>
/// 
/// </summary>
struct ImageParameters {
	VkImage                       Value;
	VkImageView                   View;
	VkSampler                     Sampler;
	VkDeviceMemory                Memory;
};
/// <summary>
/// 
/// </summary>
struct SwapChainParameters {
	VkSwapchainKHR                Value;
	VkFormat                      Format;
	struct ImageParameters* Images;
	int							  ImagesCount;
	VkExtent2D                    Extent;
};

/// <summary>
/// 
/// </summary>
struct vkewContext
{
	void* ctx;
	VkApplicationInfo appInfo;
	VkColorSpaceKHR colorSpace;
	VkCommandBuffer* presentQueueCmdBuffers;
	VkDebugReportCallbackEXT messageCallback;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkExtensionProperties* availableExtensions;
	VkFormat colorFormat;
	VkInstanceCreateInfo instanceCreateInfo;
	VkInterface i;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceSurfaceInfo2KHR physicalDeviceSurfaceInfo2KHR;
	VkPresentModeKHR* presentationModes;
	VkQueue queue;
	VkQueue transfertQueue;
	VkSurfaceFormatKHR* surfaceFormats;
#ifdef VK_EXT_full_screen_exclusive
	VkSurfaceFullScreenExclusiveInfoEXT surfaceFullScreenExclusiveInfoEXT;
	VkSurfaceCapabilities2KHR surfaceCapabilities2KHR;
	VkSurfaceFullScreenExclusiveWin32InfoEXT surfaceFullScreenExclusiveWin32InfoEXT;
#endif
	VkSurfaceKHR presentationSurface;
	const char** enabledExtensions;
	const char** validationLayerNames;
	int availableExtensionCount;
	int enableValidation;
	int enabledExtensionCount;
	int presentQueueCmdBuffersCount;
	int separatePresentQueue;
	int supportsExclusiveFullscreen;
	int validationLayerCount;
	size_t nodeIndex;
	struct SwapChainParameters swapChainParams;
	uint32_t current_buffer;
	uint32_t formatCount;
	uint32_t frame_index;
	uint32_t frame_rendered;
	uint32_t graphicsQueueFamilyIndex;
	uint32_t numPresentationModes;
	uint32_t presentQueueFamilyIndex;
	uint32_t transfertQueueFamilyIndex;
	uint32_t queueNodeIndex;

	uint32_t queueTransfertIndex;


};
typedef struct vkewContext VKEWContext;
static VKEWContext Vulkan;

static void AddExtensionLayer(const char* name)
{
	Vulkan.enabledExtensions[Vulkan.enabledExtensionCount] = name;
	Vulkan.enabledExtensionCount++;
}
static int HasExtensionLayer(const char* name)
{
	for (int i = 0; i < Vulkan.enabledExtensionCount; i++)
		if (!strcmp(Vulkan.enabledExtensions[i], name))
			return 1;
	return 0;
}



static void* vkewGetProcAddress(const char* name)
{
#ifdef WIN32
	return  GetProcAddress((HMODULE)Vulkan.ctx, name);
#elif defined UNIX
	return dlsym(Vulkan.ctx, (const char*)name);
#else
	return NULL;
#endif
}

static int _vkewStrLen(const char* const* s)
{
	int i = 0;
	if (s == NULL) return VK_FALSE;
	while (s[i] != '\0') i++;
	return i;
}
static int _vkewStrCLen(const char* s, char c)
{
	int i = 0;
	if (s == NULL) return VK_FALSE;
	while (s[i] != '\0' && s[i] != c) i++;
	return (s[i] == '\0' || s[i] == c) ? i : 0;
}
#define _vkewResolveProc(s) s = (void*)vkewGetProcAddress(#s)
#define VKEW_GET_PROC(name) PFN_##name name
#define VKEW_GET_FUNCTION(name) PFN_##name name
#ifdef VK_KHR_android_surface
static int vkewInit_VK_KHR_android_surface(VkDevice device)
{
	VKEW_GET_FUNCTION(vkCreateAndroidSurfaceKHR);
	return VK_TRUE;
}
#endif
#if defined VK_NO_PROTOTYPES

VKEW_GET_PROC(vkCreateInstance);
VKEW_GET_PROC(vkEnumerateInstanceExtensionProperties);
VKEW_GET_PROC(vkEnumerateInstanceLayerProperties);
VKEW_GET_PROC(vkGetDeviceProcAddr);
VKEW_GET_PROC(vkGetInstanceProcAddr);

VKEW_GET_FUNCTION(vkGetPhysicalDeviceFeatures);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceFormatProperties);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceProperties);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);


VKEW_GET_FUNCTION(vkAllocateCommandBuffers);
VKEW_GET_FUNCTION(vkAllocateDescriptorSets);
VKEW_GET_FUNCTION(vkAllocateMemory);
VKEW_GET_FUNCTION(vkBeginCommandBuffer);
VKEW_GET_FUNCTION(vkBindBufferMemory);
VKEW_GET_FUNCTION(vkBindImageMemory);
VKEW_GET_FUNCTION(vkCmdBeginQuery);
VKEW_GET_FUNCTION(vkCmdBeginRenderPass);
VKEW_GET_FUNCTION(vkCmdBindDescriptorSets);
VKEW_GET_FUNCTION(vkCmdBindIndexBuffer);
VKEW_GET_FUNCTION(vkCmdBindPipeline);
VKEW_GET_FUNCTION(vkCmdBindVertexBuffers);
VKEW_GET_FUNCTION(vkCmdBlitImage);
VKEW_GET_FUNCTION(vkCmdClearAttachments);
VKEW_GET_FUNCTION(vkCmdClearColorImage);
VKEW_GET_FUNCTION(vkCmdCopyBuffer);
VKEW_GET_FUNCTION(vkCmdCopyBufferToImage);
VKEW_GET_FUNCTION(vkCmdCopyImageToBuffer);
VKEW_GET_FUNCTION(vkCmdCopyImage);
VKEW_GET_FUNCTION(vkCmdCopyQueryPoolResults);
VKEW_GET_FUNCTION(vkCmdDispatch);
VKEW_GET_FUNCTION(vkCmdDraw);
VKEW_GET_FUNCTION(vkCmdDrawIndexed);
VKEW_GET_FUNCTION(vkCmdDrawIndexedIndirect);
VKEW_GET_FUNCTION(vkCmdDrawIndirect);
VKEW_GET_FUNCTION(vkCmdEndQuery);
VKEW_GET_FUNCTION(vkCmdEndRenderPass);
VKEW_GET_FUNCTION(vkCmdExecuteCommands);
VKEW_GET_FUNCTION(vkCmdPipelineBarrier);
VKEW_GET_FUNCTION(vkCmdPushConstants);
VKEW_GET_FUNCTION(vkCmdResetQueryPool);
VKEW_GET_FUNCTION(vkCmdSetDepthBias);
VKEW_GET_FUNCTION(vkCmdSetLineWidth);
VKEW_GET_FUNCTION(vkCmdSetScissor);
VKEW_GET_FUNCTION(vkCmdSetViewport);
VKEW_GET_FUNCTION(vkCreateBuffer);
VKEW_GET_FUNCTION(vkCreateCommandPool);
VKEW_GET_FUNCTION(vkCreateComputePipelines);
VKEW_GET_FUNCTION(vkCreateDescriptorPool);
VKEW_GET_FUNCTION(vkCreateDescriptorPool);  // ?
VKEW_GET_FUNCTION(vkCreateDescriptorSetLayout);
VKEW_GET_FUNCTION(vkCreateDevice);
VKEW_GET_FUNCTION(vkCreateFence);
VKEW_GET_FUNCTION(vkCreateFramebuffer);
VKEW_GET_FUNCTION(vkCreateGraphicsPipelines);
VKEW_GET_FUNCTION(vkCreateImage);
VKEW_GET_FUNCTION(vkCreateImageView);
VKEW_GET_FUNCTION(vkCreatePipelineCache);
VKEW_GET_FUNCTION(vkCreatePipelineLayout);
VKEW_GET_FUNCTION(vkCreateQueryPool);
VKEW_GET_FUNCTION(vkCreateRenderPass);
VKEW_GET_FUNCTION(vkCreateSampler);
VKEW_GET_FUNCTION(vkCreateSemaphore);
VKEW_GET_FUNCTION(vkWaitSemaphores);
VKEW_GET_FUNCTION(vkCreateShaderModule);
VKEW_GET_FUNCTION(vkDestroyBuffer);
VKEW_GET_FUNCTION(vkDestroyCommandPool);
VKEW_GET_FUNCTION(vkDestroyDescriptorPool);
VKEW_GET_FUNCTION(vkDestroyDescriptorSetLayout);
VKEW_GET_FUNCTION(vkDestroyDevice);
VKEW_GET_FUNCTION(vkDestroyFence);
VKEW_GET_FUNCTION(vkDestroyFramebuffer);
VKEW_GET_FUNCTION(vkDestroyImage);
VKEW_GET_FUNCTION(vkDestroyImageView);
VKEW_GET_FUNCTION(vkDestroyInstance);
VKEW_GET_FUNCTION(vkDestroyPipeline);
VKEW_GET_FUNCTION(vkDestroyPipelineCache);
VKEW_GET_FUNCTION(vkDestroyPipelineLayout);
VKEW_GET_FUNCTION(vkDestroyQueryPool);
VKEW_GET_FUNCTION(vkDestroyRenderPass);
VKEW_GET_FUNCTION(vkDestroySampler);
VKEW_GET_FUNCTION(vkDestroySemaphore);
VKEW_GET_FUNCTION(vkDestroyShaderModule);
VKEW_GET_FUNCTION(vkDeviceWaitIdle);
VKEW_GET_FUNCTION(vkEndCommandBuffer);
VKEW_GET_FUNCTION(vkEnumerateDeviceExtensionProperties);
VKEW_GET_FUNCTION(vkEnumerateDeviceLayerProperties);
VKEW_GET_FUNCTION(vkEnumeratePhysicalDevices);
VKEW_GET_FUNCTION(vkFlushMappedMemoryRanges);
VKEW_GET_FUNCTION(vkFreeCommandBuffers);
VKEW_GET_FUNCTION(vkFreeDescriptorSets);
VKEW_GET_FUNCTION(vkFreeMemory);
VKEW_GET_FUNCTION(vkGetBufferMemoryRequirements);
VKEW_GET_FUNCTION(vkGetDeviceQueue);
VKEW_GET_FUNCTION(vkGetFenceStatus);
VKEW_GET_FUNCTION(vkGetImageMemoryRequirements);
VKEW_GET_FUNCTION(vkGetImageSubresourceLayout);
VKEW_GET_FUNCTION(vkGetQueryPoolResults);
VKEW_GET_FUNCTION(vkInvalidateMappedMemoryRanges);
VKEW_GET_FUNCTION(vkMapMemory);
VKEW_GET_FUNCTION(vkQueueSubmit);
VKEW_GET_FUNCTION(vkQueueWaitIdle);
VKEW_GET_FUNCTION(vkResetDescriptorPool);
VKEW_GET_FUNCTION(vkResetFences);
VKEW_GET_FUNCTION(vkResetCommandPool);
VKEW_GET_FUNCTION(vkUnmapMemory);
VKEW_GET_FUNCTION(vkUpdateDescriptorSets);
VKEW_GET_FUNCTION(vkWaitForFences);

VKEW_GET_FUNCTION(vkGetBufferMemoryRequirements2KHR);
VKEW_GET_FUNCTION(vkGetBufferMemoryRequirements2);
VKEW_GET_FUNCTION(vkBindBufferMemory2);
VKEW_GET_FUNCTION(vkBindImageMemory2);
VKEW_GET_FUNCTION(vkGetImageMemoryRequirements2);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceMemoryProperties2);



#ifdef VK_KHR_xlib_surface
VKEW_GET_FUNCTION(vkCreateXlibSurfaceKHR);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceXlibPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_XLIB_KHR */
#ifdef VK_KHR_xcb_surface
VKEW_GET_FUNCTION(vkCreateXcbSurfaceKHR);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceXcbPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_XCB_KHR */	
#ifdef VK_KHR_wayland_surface
VKEW_GET_FUNCTION(vkCreateWaylandSurfaceKHR);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceWaylandPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_WAYLAND_KHR */
#ifdef VK_KHR_mir_surface
VKEW_GET_FUNCTION(vkCreateMirSurfaceKHR);
VKEW_GET_FUNCTION(vkGetPhysicalDeviceMirPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_MIR_KHR */
#endif
VkBool32 _vkewCheckExtensionAvailability(const char* extension_name, const VkExtensionProperties* available_extensions, int count) {
	for (int i = 0; i < count; ++i) {
		if (strcmp(available_extensions[i].extensionName, extension_name) == 0) {
			return VK_TRUE;
		}
	}
	return VK_FALSE;
}
VkResult vkewCheckExtensionAvailability(const char* extension_name) {
	return _vkewCheckExtensionAvailability(extension_name, Vulkan.availableExtensions, Vulkan.availableExtensionCount);
}
#undef VKEW_GET_PROC
#undef VKEW_GET_FUNCTION
#undef VKEW_GET_FUNCTION
#define VKEW_GET_PROC(name) name = (PFN_##name)(vkewGetProcAddress(#name))
#define VKEW_GET_FUNCTION(name) name = (PFN_##name)(vkewGetInterfaceProcAddr(value, #name))
#ifdef VK_EXT_debug_marker
#if defined VK_NO_PROTOTYPES
PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif
VkBool32 VKEW_EXT_debug_marker;
static int vkewInit_VK_EXT_debug_markerVK_EXT_debug_marker(VkInterface value)
{
	if (!HasExtensionLayer(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
		return 0;
	VKEW_GET_FUNCTION(vkCmdDebugMarkerBeginEXT);
	VKEW_GET_FUNCTION(vkCmdDebugMarkerEndEXT);
	VKEW_GET_FUNCTION(vkCmdDebugMarkerInsertEXT);
	VKEW_GET_FUNCTION(vkDebugMarkerSetObjectNameEXT);
	VKEW_GET_FUNCTION(vkDebugMarkerSetObjectTagEXT);
	return vkCmdDebugMarkerBeginEXT != NULL;
}
#endif
#ifdef VK_KHR_get_surface_capabilities2 
VkBool32 VKEW_KHR_get_surface_capabilities2;
#if defined VK_NO_PROTOTYPES
PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif
static int vkewInit_VK_KHR_get_surface_capabilities2(VkInterface value)
{
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilities2KHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceSurfaceFormats2KHR);
	return vkGetPhysicalDeviceSurfaceFormats2KHR != NULL;
}
#endif

#ifdef VK_KHR_synchronization2
VkBool32 VKEW_VK_KHR_synchronization2;
#if defined VK_NO_PROTOTYPES
PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;

#endif
static int vkewInit_VK_KHR_synchronization2(VkInterface value)
{
	VKEW_GET_FUNCTION(vkCmdPipelineBarrier2KHR);
	VKEW_GET_FUNCTION(vkCmdResetEvent2KHR);
	VKEW_GET_FUNCTION(vkCmdSetEvent2KHR);
	VKEW_GET_FUNCTION(vkCmdWaitEvents2KHR);
	VKEW_GET_FUNCTION(vkCmdWriteTimestamp2KHR);
	VKEW_GET_FUNCTION(vkQueueSubmit2KHR);

	return vkQueueSubmit2KHR != NULL;
}
#endif

#ifdef VK_EXT_robustness2
VkBool32 VKEW_EXT_robustness2;

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
static int vkewInit_VK_EXT_full_screen_exclusive(VkInterface value)
{
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceSurfacePresentModes2EXT);
	VKEW_GET_FUNCTION(vkGetDeviceGroupSurfacePresentModes2EXT);
	VKEW_GET_FUNCTION(vkAcquireFullScreenExclusiveModeEXT); // FIXME: Access violation executing location 0x0000000000000000.
	VKEW_GET_FUNCTION(vkReleaseFullScreenExclusiveModeEXT); // FIXME: Access violation executing location 0x0000000000000000.
	return vkGetPhysicalDeviceSurfacePresentModes2EXT != NULL;
}
#endif
#ifdef VK_EXT_debug_report
VkBool32 VKEW_EXT_debug_report;
#if defined VK_NO_PROTOTYPES
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
#endif
static int vkewInit_VK_EXT_debug_report(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkDestroyDebugReportCallbackEXT);
	VKEW_GET_FUNCTION(vkCreateDebugReportCallbackEXT);
#endif
	return vkCreateDebugReportCallbackEXT != NULL;
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
static int vkewInit_VK_EXT_debug_utils(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkCreateDebugUtilsMessengerEXT);
	VKEW_GET_FUNCTION(vkDestroyDebugUtilsMessengerEXT);
	VKEW_GET_FUNCTION(vkSubmitDebugUtilsMessageEXT);
	VKEW_GET_FUNCTION(vkCmdBeginDebugUtilsLabelEXT);
	VKEW_GET_FUNCTION(vkCmdEndDebugUtilsLabelEXT);
	VKEW_GET_FUNCTION(vkCmdInsertDebugUtilsLabelEXT);
	VKEW_GET_FUNCTION(vkSetDebugUtilsObjectNameEXT);
#endif
	return vkCreateDebugUtilsMessengerEXT != NULL;
}
#endif
#ifdef VK_KHR_display
#if defined VK_NO_PROTOTYPES
PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
#endif
VkBool32 VKEW_KHR_display;
static int vkewInit_VK_KHR_display(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceDisplayPropertiesKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceDisplayPlanePropertiesKHR);
	VKEW_GET_FUNCTION(vkGetDisplayPlaneSupportedDisplaysKHR);
	VKEW_GET_FUNCTION(vkGetDisplayModePropertiesKHR);
	VKEW_GET_FUNCTION(vkCreateDisplayModeKHR);
	VKEW_GET_FUNCTION(vkGetDisplayPlaneCapabilitiesKHR);
	VKEW_GET_FUNCTION(vkCreateDisplayPlaneSurfaceKHR);
#endif
	return vkGetPhysicalDeviceDisplayPropertiesKHR != NULL;
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
static int vkewInit_VK_KHR_surface(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkDestroySurfaceKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
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
static int vkewInit_VK_KHR_swapchain(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkCreateSwapchainKHR);
	VKEW_GET_FUNCTION(vkDestroySwapchainKHR);
	VKEW_GET_FUNCTION(vkGetSwapchainImagesKHR);
	VKEW_GET_FUNCTION(vkAcquireNextImageKHR);
	VKEW_GET_FUNCTION(vkQueuePresentKHR);
#endif
	return VK_TRUE;
}
#endif /* VK_KHR_swapchain  */
#ifdef VK_KHR_display_swapchain
#if defined VK_NO_PROTOTYPES
PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif
static int vkewInit_VK_KHR_display_swapchain(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkCreateSharedSwapchainsKHR);
#endif
	return VK_TRUE;
}
#endif
#ifdef VK_KHR_win32_surface
#if defined VK_NO_PROTOTYPES
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif
static int vkewInit_VK_KHR_win32_surface(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkCreateWin32SurfaceKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceWin32PresentationSupportKHR);
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
int vkewInterfaceLevelInit(VkInterface value)
{
#if defined VK_NO_PROTOTYPES
	VKEW_GET_FUNCTION(vkCreateDevice);
	VKEW_GET_FUNCTION(vkEnumerateDeviceExtensionProperties);
	VKEW_GET_FUNCTION(vkEnumerateDeviceLayerProperties);
	VKEW_GET_FUNCTION(vkEnumeratePhysicalDevices);
	VKEW_GET_FUNCTION(vkGetImageMemoryRequirements);
	VKEW_GET_FUNCTION(vkGetImageSubresourceLayout);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceFeatures);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceFormatProperties);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceProperties);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);

#ifdef VK_KHR_win32_surface
	vkewInit_VK_KHR_win32_surface(value);
#endif /* VK_USE_PLATFORM_WIN32_KHR */
#ifdef VK_KHR_surface
	vkewInit_VK_KHR_surface(value);
#endif
#ifdef VK_KHR_display
	VKEW_KHR_display = vkewInit_VK_KHR_display(value);
#endif

#ifdef VK_KHR_swapchain
	vkewInit_VK_KHR_swapchain(value);
#endif
#ifdef VK_KHR_display_swapchain
	vkewInit_VK_KHR_display_swapchain(value);
#endif
#ifdef VK_EXT_debug_report 
	VKEW_EXT_debug_report = vkewInit_VK_EXT_debug_report(value);
#endif
#ifdef VK_EXT_debug_report
	VKEW_EXT_debug_utils = vkewInit_VK_EXT_debug_utils(value);
#endif
#ifdef VK_KHR_get_surface_capabilities2
	VKEW_KHR_get_surface_capabilities2 = vkewInit_VK_KHR_get_surface_capabilities2(value);
#endif

#ifdef VK_EXT_full_screen_exclusive
	VKEW_EXT_full_screen_exclusive = vkewInit_VK_EXT_full_screen_exclusive(value);
#endif


#ifdef VK_EXT_debug_marker
	VKEW_EXT_debug_marker = 0; // Not ready for prime time : null pointer exception vkewInit_VK_EXT_debug_marker(value);
#endif

#if defined VK_NO_PROTOTYPES

	VKEW_GET_FUNCTION(vkAllocateCommandBuffers);
	VKEW_GET_FUNCTION(vkAllocateDescriptorSets);
	VKEW_GET_FUNCTION(vkAllocateMemory);
	VKEW_GET_FUNCTION(vkBeginCommandBuffer);
	VKEW_GET_FUNCTION(vkBindBufferMemory);
	VKEW_GET_FUNCTION(vkBindImageMemory);
	VKEW_GET_FUNCTION(vkCmdBeginQuery);
	VKEW_GET_FUNCTION(vkCmdBeginRenderPass);
	VKEW_GET_FUNCTION(vkCmdBindDescriptorSets);
	VKEW_GET_FUNCTION(vkCmdBindIndexBuffer);
	VKEW_GET_FUNCTION(vkCmdBindPipeline);
	VKEW_GET_FUNCTION(vkCmdBindVertexBuffers);
	VKEW_GET_FUNCTION(vkCmdBlitImage);
	VKEW_GET_FUNCTION(vkCmdClearAttachments);
	VKEW_GET_FUNCTION(vkCmdClearColorImage);
	VKEW_GET_FUNCTION(vkCmdCopyBuffer);
	VKEW_GET_FUNCTION(vkCmdCopyBufferToImage);
	VKEW_GET_FUNCTION(vkCmdCopyImageToBuffer);
	VKEW_GET_FUNCTION(vkCmdCopyImage);
	VKEW_GET_FUNCTION(vkCmdCopyQueryPoolResults);
	VKEW_GET_FUNCTION(vkCmdDispatch);
	VKEW_GET_FUNCTION(vkCmdDraw);
	VKEW_GET_FUNCTION(vkCmdDrawIndexed);
	VKEW_GET_FUNCTION(vkCmdDrawIndexedIndirect);
	VKEW_GET_FUNCTION(vkCmdDrawIndirect);
	VKEW_GET_FUNCTION(vkCmdEndQuery);
	VKEW_GET_FUNCTION(vkCmdEndRenderPass);
	VKEW_GET_FUNCTION(vkCmdExecuteCommands);
	VKEW_GET_FUNCTION(vkCmdPipelineBarrier);
	VKEW_GET_FUNCTION(vkCmdPushConstants);
	VKEW_GET_FUNCTION(vkCmdResetQueryPool);
	VKEW_GET_FUNCTION(vkCmdSetDepthBias);
	VKEW_GET_FUNCTION(vkCmdSetLineWidth);
	VKEW_GET_FUNCTION(vkCmdSetScissor);
	VKEW_GET_FUNCTION(vkCmdSetViewport);
	VKEW_GET_FUNCTION(vkCreateBuffer);
	VKEW_GET_FUNCTION(vkCreateCommandPool);
	VKEW_GET_FUNCTION(vkCreateComputePipelines);
	VKEW_GET_FUNCTION(vkCreateDescriptorPool);
	VKEW_GET_FUNCTION(vkCreateDescriptorPool);  // ?
	VKEW_GET_FUNCTION(vkCreateDescriptorSetLayout);
	VKEW_GET_FUNCTION(vkCreateDevice);
	VKEW_GET_FUNCTION(vkCreateFence);
	VKEW_GET_FUNCTION(vkCreateFramebuffer);
	VKEW_GET_FUNCTION(vkCreateGraphicsPipelines);
	VKEW_GET_FUNCTION(vkCreateImage);
	VKEW_GET_FUNCTION(vkCreateImageView);
	VKEW_GET_FUNCTION(vkCreatePipelineCache);
	VKEW_GET_FUNCTION(vkCreatePipelineLayout);
	VKEW_GET_FUNCTION(vkCreateQueryPool);
	VKEW_GET_FUNCTION(vkCreateRenderPass);
	VKEW_GET_FUNCTION(vkCreateSampler);
	VKEW_GET_FUNCTION(vkCreateSemaphore);
	VKEW_GET_FUNCTION(vkWaitSemaphores);
	VKEW_GET_FUNCTION(vkCreateShaderModule);
	VKEW_GET_FUNCTION(vkDestroyBuffer);
	VKEW_GET_FUNCTION(vkDestroyCommandPool);
	VKEW_GET_FUNCTION(vkDestroyDescriptorPool);
	VKEW_GET_FUNCTION(vkDestroyDescriptorSetLayout);
	VKEW_GET_FUNCTION(vkDestroyDevice);
	VKEW_GET_FUNCTION(vkDestroyFence);
	VKEW_GET_FUNCTION(vkDestroyFramebuffer);
	VKEW_GET_FUNCTION(vkDestroyImage);
	VKEW_GET_FUNCTION(vkDestroyImageView);
	VKEW_GET_FUNCTION(vkDestroyInstance);
	VKEW_GET_FUNCTION(vkDestroyPipeline);
	VKEW_GET_FUNCTION(vkDestroyPipelineCache);
	VKEW_GET_FUNCTION(vkDestroyPipelineLayout);
	VKEW_GET_FUNCTION(vkDestroyQueryPool);
	VKEW_GET_FUNCTION(vkDestroyRenderPass);
	VKEW_GET_FUNCTION(vkDestroySampler);
	VKEW_GET_FUNCTION(vkDestroySemaphore);
	VKEW_GET_FUNCTION(vkDestroyShaderModule);
	VKEW_GET_FUNCTION(vkDeviceWaitIdle);
	VKEW_GET_FUNCTION(vkEndCommandBuffer);
	VKEW_GET_FUNCTION(vkFlushMappedMemoryRanges);
	VKEW_GET_FUNCTION(vkFreeCommandBuffers);
	VKEW_GET_FUNCTION(vkFreeDescriptorSets);
	VKEW_GET_FUNCTION(vkFreeMemory);
	VKEW_GET_FUNCTION(vkGetBufferMemoryRequirements);
	VKEW_GET_FUNCTION(vkGetDeviceQueue);

	VKEW_GET_FUNCTION(vkGetFenceStatus);
	VKEW_GET_FUNCTION(vkGetImageMemoryRequirements);
	VKEW_GET_FUNCTION(vkGetImageSubresourceLayout);
	VKEW_GET_FUNCTION(vkGetQueryPoolResults);
	VKEW_GET_FUNCTION(vkInvalidateMappedMemoryRanges);
	VKEW_GET_FUNCTION(vkMapMemory);
	VKEW_GET_FUNCTION(vkQueueSubmit);
	VKEW_GET_FUNCTION(vkQueueWaitIdle);
	VKEW_GET_FUNCTION(vkResetDescriptorPool);
	VKEW_GET_FUNCTION(vkResetFences);
	VKEW_GET_FUNCTION(vkResetCommandPool);
	VKEW_GET_FUNCTION(vkUnmapMemory);
	VKEW_GET_FUNCTION(vkUpdateDescriptorSets);
	VKEW_GET_FUNCTION(vkWaitForFences);

	VKEW_GET_FUNCTION(vkGetBufferMemoryRequirements2KHR);
	VKEW_GET_FUNCTION(vkGetBufferMemoryRequirements2);
	VKEW_GET_FUNCTION(vkBindBufferMemory2);
	VKEW_GET_FUNCTION(vkBindImageMemory2);
	VKEW_GET_FUNCTION(vkGetImageMemoryRequirements2);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceMemoryProperties2);

#endif
#ifdef VK_EXT_full_screen_exclusive
#endif
#ifdef VK_KHR_android_surface
    VKEW_GET_FUNCTION(vkCreateAndroidSurfaceKHR);
#endif /* VK_USE_PLATFORM_ANDROID_KHR */
#ifdef VK_KHR_xlib_surface
	VKEW_GET_FUNCTION(vkCreateXlibSurfaceKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceXlibPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_XLIB_KHR */
#ifdef VK_KHR_xcb_surface
	VKEW_GET_FUNCTION(vkCreateXcbSurfaceKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceXcbPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_XCB_KHR */	
#ifdef VK_KHR_wayland_surface
	VKEW_GET_FUNCTION(vkCreateWaylandSurfaceKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceWaylandPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_WAYLAND_KHR */
#ifdef VK_KHR_mir_surface
	VKEW_GET_FUNCTION(vkCreateMirSurfaceKHR);
	VKEW_GET_FUNCTION(vkGetPhysicalDeviceMirPresentationSupportKHR);
#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_KHR_synchronization2
	VKEW_VK_KHR_synchronization2 = vkewInit_VK_KHR_synchronization2(value);
#endif
#endif
	return VK_FALSE;
}
int vkewSupportsFullscreenExclusive(void)
{
	return Vulkan.supportsExclusiveFullscreen;
}
void vkewDestroy(void)
{
	
	if (Vulkan.i.device != VK_NULL_HANDLE) {
		vkewReleaseSwapChain();
		vkDeviceWaitIdle(Vulkan.i.device);
		vkDestroyDevice(Vulkan.i.device, NULL);
		Vulkan.i.device = VK_NULL_HANDLE;
	}

	if (Vulkan.i.instance != VK_NULL_HANDLE) {

		if (Vulkan.presentationSurface != VK_NULL_HANDLE) {
			vkDestroySurfaceKHR(Vulkan.i.instance, Vulkan.presentationSurface, NULL);
			Vulkan.presentationSurface = VK_NULL_HANDLE;
		}

		if (Vulkan.debugMessenger != VK_NULL_HANDLE) {
			vkDestroyDebugUtilsMessengerEXT(Vulkan.i.instance, Vulkan.debugMessenger, NULL);
		}

		vkDestroyInstance(Vulkan.i.instance, NULL);
		Vulkan.i.instance = VK_NULL_HANDLE;
	}
#if defined(_WIN32)
	FreeLibrary(Vulkan.ctx);
#else
	dlclose(Vulkan.ctx);
#endif
	memset(&Vulkan, 0, sizeof(Vulkan));
}
VkInstance vkewGetInstance()
{
	return Vulkan.i.instance;
}
static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(
	VkDebugReportFlagsEXT                       msgFlags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     msgCode,
	const char* pLayerPrefix,
	const char* pMsg,
	void* pUserData)
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
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {
#if 1
	char prefix[64] = "";
	size_t messageLen = strlen(pCallbackData->pMessage) + 5000;
	char* message = (char*)malloc(messageLen);
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		l_strncat(prefix, sizeof(prefix), "VERBOSE : ");
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		l_strncat(prefix, sizeof(prefix), "INFO : ");
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		l_strncat(prefix, sizeof(prefix), "WARNING : ");
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		l_strncat(prefix, sizeof(prefix), "ERROR : ");
	}
	if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
		l_strncat(prefix, sizeof(prefix), "GENERAL");
	}
	else {
		if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
			l_strncat(prefix, sizeof(prefix), "VALIDATION");
		}
		if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
			if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
				l_strncat(prefix, sizeof(prefix), "|");
			}
			l_strncat(prefix, sizeof(prefix), "PERFORMANCE");
		}
	}
	l_snprintf(message, messageLen, "%s - Message Id Number: %d | Message Id Name: %s\n\t%s\n", prefix, pCallbackData->messageIdNumber,
		pCallbackData->pMessageIdName, pCallbackData->pMessage);
	if (pCallbackData->objectCount > 0) {
		char tmp_message[500];
		l_snprintf(tmp_message, sizeof(tmp_message), "\n\tObjects - %d\n", pCallbackData->objectCount);
		l_strncat(message, messageLen, tmp_message);
		for (uint32_t object = 0; object < pCallbackData->objectCount; ++object) {
			if (NULL != pCallbackData->pObjects[object].pObjectName && strlen(pCallbackData->pObjects[object].pObjectName) > 0) {
				l_snprintf(tmp_message, sizeof(tmp_message), "\t\tObject[%d] - %s, Handle %p, Name \"%s\"\n", object,
					vkObjectType_ToString(pCallbackData->pObjects[object].objectType),
					(void*)(pCallbackData->pObjects[object].objectHandle), pCallbackData->pObjects[object].pObjectName);
			}
			else {
				l_snprintf(tmp_message, sizeof(tmp_message), "\t\tObject[%d] - %s, Handle %p\n", object,
					vkObjectType_ToString(pCallbackData->pObjects[object].objectType),
					(void*)(pCallbackData->pObjects[object].objectHandle));
			}
			l_strncat(message, messageLen, tmp_message);
		}
	}
	if (pCallbackData->cmdBufLabelCount > 0) {
		char tmp_message[500];
		l_snprintf(tmp_message, sizeof(tmp_message), "\n\tCommand Buffer Labels - %d\n", pCallbackData->cmdBufLabelCount);
		l_strncat(message, messageLen, tmp_message);
		for (uint32_t cmd_buf_label = 0; cmd_buf_label < pCallbackData->cmdBufLabelCount; ++cmd_buf_label) {
			l_snprintf(tmp_message, sizeof(tmp_message), "\t\tLabel[%d] - %s { %f, %f, %f, %f}\n", cmd_buf_label,
				pCallbackData->pCmdBufLabels[cmd_buf_label].pLabelName, pCallbackData->pCmdBufLabels[cmd_buf_label].color[0],
				pCallbackData->pCmdBufLabels[cmd_buf_label].color[1], pCallbackData->pCmdBufLabels[cmd_buf_label].color[2],
				pCallbackData->pCmdBufLabels[cmd_buf_label].color[3]);
			l_strncat(message, messageLen, tmp_message);
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
static VkBool32 _vkewCheckLayerAvailabilty(const uint32_t check_count, const char** check_names, const uint32_t layer_count, const VkLayerProperties* layers) {
	for (uint32_t i = 0; i < check_count; i++) {
		VkBool32 found = 0;
		for (uint32_t j = 0; j < layer_count; j++) {
			if (!strcmp(check_names[i], layers[j].layerName)) {
				found = 1;
				break;
			}
		}
		if (!found) {
			vkewLogMessage("<WARNING> Layer '%s' not found\n", check_names[i]);
			return 0;
		}
	}
	return 1;
}
static int InitValidationLayers(int enableValidation)
{
	if (!enableValidation) {
		return enableValidation;
	}
	uint32_t instance_extension_count = 0;
	uint32_t instance_layer_count = 0;
	uint32_t validation_layer_count = 0;
	int validation_found;
	static const char* instance_validation_layers_alt1[] = { "VK_LAYER_KHRONOS_validation" };
	static const char* instance_validation_layers_alt2[] = { "VK_LAYER_LUNARG_standard_validation" };
	static const char* instance_validation_layers_alt3[] = { "VK_LAYER_GOOGLE_threading",
															 "VK_LAYER_LUNARG_parameter_validation",
															 "VK_LAYER_LUNARG_object_tracker",
															 "VK_LAYER_LUNARG_core_validation",
															 "VK_LAYER_GOOGLE_unique_objects"
	};
	VkResult err = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
	const char** it = instance_validation_layers_alt1;
	if (instance_layer_count > 0) {
		VkLayerProperties* instance_layers = malloc(sizeof(VkLayerProperties) * instance_layer_count);
		err = vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layers);
		validation_found = _vkewCheckLayerAvailabilty(ARRAY_SIZE(instance_validation_layers_alt1), it,
			instance_layer_count, instance_layers);
		if (validation_found)
		{
			Vulkan.validationLayerCount = ARRAY_SIZE(instance_validation_layers_alt1);
			Vulkan.validationLayerNames[0] = instance_validation_layers_alt1[0];
		}
		else
		{
			validation_found = _vkewCheckLayerAvailabilty(ARRAY_SIZE(instance_validation_layers_alt2), it,
				instance_layer_count, instance_layers);
			if (validation_found)
			{
				Vulkan.validationLayerCount = ARRAY_SIZE(instance_validation_layers_alt2);
				Vulkan.validationLayerNames[0] = instance_validation_layers_alt2[0];
			}
			else
			{
				// use alternative set of validation layers
				it = instance_validation_layers_alt3;
				Vulkan.validationLayerCount = ARRAY_SIZE(instance_validation_layers_alt3);
				validation_found = _vkewCheckLayerAvailabilty(ARRAY_SIZE(instance_validation_layers_alt3), it,
					instance_layer_count, instance_layers);
				if (validation_found)
				{
					for (int32_t i = 0; i < Vulkan.validationLayerCount; i++) {
						Vulkan.validationLayerNames[i] = it[i];
					}
				}
			}
		}
		free(instance_layers);
		return validation_found;
	}
	return 0;
}

static void InitExtensionsLayers(int enableValidation)
{
	int platformSurfaceExtFound = 0;
	uint32_t instance_extension_count = 0;
	int surfaceExtFound = 0;
	int surfaceCaps2Ext;
	int have_EXT_robustness2_features = 0;
	VkResult err = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, NULL);
	if (instance_extension_count > 0) {
		VkExtensionProperties* instance_extensions = malloc(sizeof(VkExtensionProperties) * instance_extension_count);
		err = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, instance_extensions);
		for (int32_t i = 0; i < (int32_t)instance_extension_count; i++) {


			// ppEnabledExtensionNames[enabledExtensionCount++] = VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME;
			if (!strcmp(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				surfaceCaps2Ext = 1;
				AddExtensionLayer(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
			}

			if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				surfaceExtFound = 1;
				AddExtensionLayer(VK_KHR_SURFACE_EXTENSION_NAME);
			}

			if (!strcmp(VK_EXT_DEBUG_MARKER_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				AddExtensionLayer(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
			}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtensionLayer(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
			}

			
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			if (!strcmp(VK_KHR_XLIB_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtensionLayer(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			if (!strcmp(VK_KHR_XCB_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtensionLayer(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
			if (!strcmp(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtensionLayer(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_MIR_KHR)
#elif defined(VK_USE_PLATFORM_DISPLAY_KHR)
			if (!strcmp(VK_KHR_DISPLAY_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtensionLayer(VK_KHR_DISPLAY_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
			if (!strcmp(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtensionLayer(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
			}
#elif defined(VK_USE_PLATFORM_IOS_MVK)
			if (!strcmp(VK_MVK_IOS_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				demo->extension_names[demo->enabled_extension_count++] = VK_MVK_IOS_SURFACE_EXTENSION_NAME;
			}
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
			if (!strcmp(VK_MVK_MACOS_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				platformSurfaceExtFound = 1;
				AddExtensionLayer(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
			}
#endif
#ifdef USE_EXT_debug_report
			// Old
			if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				if (enableValidation) {
					AddExtensionLayer(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
				}
			}
#else
			// New
			if (!strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, instance_extensions[i].extensionName)) {
				if (enableValidation) {
					AddExtensionLayer(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
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
		VkResult err = vkCreateDebugUtilsMessengerEXT(Vulkan.i.instance, &s_CreateInfoUtilMessenger, NULL, &Vulkan.debugMessenger);

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
		Vulkan.i.instanceCreateInfo.pNext = &s_CreateInfoDebugReport;
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
#elif defined(UNIX)
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
	enableValidation = InitValidationLayers(enableValidation);
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
	err = VK_CHECK(vkCreateInstance(&Vulkan.instanceCreateInfo,
		NULL,  // If pAllocator is not NULL, pAllocator must be a pointer to a valid VkAllocationCallbacks structure
		&Vulkan.i.instance)); //pInstance points a VkInstance handle in which the resulting instance is returned.
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
		vkewInterfaceLevelInit(Vulkan.i);
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



VkBool32 vkewCheckPhysicalDeviceProperties(VkPhysicalDevice physical_device,
	uint32_t* selected_graphics_queue_family_index,
	uint32_t* selected_present_queue_family_index,
	uint32_t* selected_transfer_queue_family_index) {
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
	VKEW_EXT_robustness2 = _vkewCheckExtensionAvailability(VK_EXT_ROBUSTNESS_2_EXTENSION_NAME, available_extensions, extensions_count);
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

	free(available_extensions);
	VkQueueFamilyProperties* queue_family_properties = (VkQueueFamilyProperties*)calloc(queue_families_count, sizeof(VkQueueFamilyProperties));
	VkBool32* queue_present_support = (VkBool32*)calloc(queue_families_count, sizeof(VkBool32));
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, &queue_family_properties[0]);
	uint32_t graphics_queue_family_index = UINT32_MAX;
	uint32_t present_queue_family_index = UINT32_MAX;
	uint32_t transfert_queue_family_index = UINT32_MAX;



	for (uint32_t i = 0; i < queue_families_count; ++i) {
		vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, Vulkan.presentationSurface, &queue_present_support[i]);
	}
	// Check for transfert only queue
	for (uint32_t i = 0; i < queue_families_count; ++i) {
		if ((queue_family_properties[i].queueCount > 0) &&
			(queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && !(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
			*selected_transfer_queue_family_index = i;
		}
	}


	// Check for graphics queue
	for (uint32_t i = 0; i < queue_families_count; ++i) {
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
	const char* pLayerPrefix,
	const char* pMsg,
	void* pUserData)
{
	(void)msgFlags; (void)objType; (void)srcObject; (void)location; (void)pUserData; (void)msgCode;
	vkewLogMessage("%s: %s\n", pLayerPrefix, pMsg);
	// If the application returns VK_TRUE from its callback and the Vulkan call being aborted returns a VkResult, the layer will return VK_ERROR_VALIDATION_FAILED_EXT.
	return msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT ? VK_TRUE : VK_FALSE;
}
VkResult vkewCreateDevice(void)
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
	uint32_t selected_transfert_queue_family_index = UINT32_MAX;
	for (uint32_t i = 0; i < num_devices; ++i) {
		if (vkewCheckPhysicalDeviceProperties(physical_devices[i],
			&selected_graphics_queue_family_index,
			&selected_present_queue_family_index,
			&selected_transfert_queue_family_index)) {
			Vulkan.physicalDevice = physical_devices[i];
		}
	}
	if (Vulkan.physicalDevice == VK_NULL_HANDLE) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	VkDeviceQueueCreateInfo queue_create_infos[3];
	float queue_priorities[] = { 1.0f };
	int queueCount = 1;
	VkDeviceQueueCreateInfo qci0 = {
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
	VkDeviceQueueCreateInfo qci2 = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
		NULL,                                        // const void                  *pNext
		0,                                              // VkDeviceQueueCreateFlags     flags
		selected_transfert_queue_family_index,            // uint32_t                     queueFamilyIndex
		queueCount, // uint32_t                     queueCount
		&queue_priorities[0]                            // const float                 *pQueuePriorities
	};
	queue_create_infos[queueCreateInfoCount++] = qci0;

	if (selected_graphics_queue_family_index != selected_present_queue_family_index) {
		queue_create_infos[queueCreateInfoCount++] = qci1;
	}
    queue_create_infos[queueCreateInfoCount++] = qci2;

	int enabledExtensionCount = 0;
	const char* ppEnabledExtensionNames[64] = { NULL };
	VkPhysicalDeviceFeatures* pEnabledFeatures = NULL;

	ppEnabledExtensionNames[enabledExtensionCount++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	VkPhysicalDeviceRobustness2FeaturesEXT robustness2 = {
			VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT
	};
	robustness2.nullDescriptor = 1;
	if (VKEW_EXT_robustness2)
	{
		ppEnabledExtensionNames[enabledExtensionCount++] = VK_EXT_ROBUSTNESS_2_EXTENSION_NAME;

	}
#ifdef VK_EXT_full_screen_exclusive
	if (VKEW_EXT_full_screen_exclusive)
	{
		ppEnabledExtensionNames[enabledExtensionCount++] = VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME;
		// Loader_validate_device_extensions: Device extension VK_KHR_get_surface_capabilities2 not supported by selected physical device or enabled layers.
		// ppEnabledExtensionNames[enabledExtensionCount++] = VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME;
	}	
#endif


	typedef struct VkPhysicalDeviceRobustness2FeaturesEXT {
		VkStructureType    sType;
		void* pNext;
		VkBool32           robustBufferAccess2;
		VkBool32           robustImageAccess2;
		VkBool32           nullDescriptor;
	} VkPhysicalDeviceRobustness2FeaturesEXT;

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
		pEnabledFeatures                                           // const VkPhysicalDeviceFeatures    *pEnabledFeatures
	};
	if (VKEW_EXT_robustness2)
	{
		device_create_info.pNext = &robustness2;
	}

	if (VK_CHECK(vkCreateDevice(Vulkan.physicalDevice, &device_create_info, NULL, &Vulkan.i.device)) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	if (Vulkan.i.device == VK_NULL_HANDLE) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	vkewInterfaceLevelInit(Vulkan.i);
	Vulkan.graphicsQueueFamilyIndex = selected_graphics_queue_family_index;
	Vulkan.presentQueueFamilyIndex = selected_present_queue_family_index;
	Vulkan.transfertQueueFamilyIndex = selected_transfert_queue_family_index;

	Vulkan.queueNodeIndex = Vulkan.graphicsQueueFamilyIndex;
	vkGetDeviceQueue(Vulkan.i.device, Vulkan.queueNodeIndex, 0, &Vulkan.queue);

	Vulkan.queueTransfertIndex = Vulkan.transfertQueueFamilyIndex;
	vkGetDeviceQueue(Vulkan.i.device, Vulkan.queueTransfertIndex, 0, &Vulkan.transfertQueue);

	//vkewCreatePresentationSemaphore();
	vkGetPhysicalDeviceProperties(Vulkan.physicalDevice, &Vulkan.deviceProperties);
	vkGetPhysicalDeviceFeatures(Vulkan.physicalDevice, &Vulkan.deviceFeatures);
	return VK_SUCCESS;
}
void vkewGetPropertiesAndFeatures(VkPhysicalDeviceProperties* p, VkPhysicalDeviceFeatures* f)
{
	*p = Vulkan.deviceProperties;
	*f = Vulkan.deviceFeatures;
}
VkResult vkewCreateSurface(int deviceIndex, void* platformHandle, void* platformWindow, VkFormat colorFormat)
{
	VkResult result;
#if defined(_WIN32)
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)platformHandle; // provided by the platform code
	surfaceCreateInfo.hwnd = (HWND)platformWindow;           // provided by the platform code
	result = VK_CHECK(vkCreateWin32SurfaceKHR(Vulkan.i.instance, &surfaceCreateInfo, NULL, &Vulkan.presentationSurface));
#elif defined(__ANDROID__)
	VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.window = platformWindow;                       // provided by the platform code
	result = vkCreateAndroidSurfaceKHR(vkewGetInstance(), &surfaceCreateInfo, NULL, &Vulkan.presentationSurface);
#elif defined (UNIX)
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

	vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &Vulkan.formatCount, NULL);
	Vulkan.surfaceFormats = (VkSurfaceFormatKHR*)calloc(Vulkan.formatCount, sizeof(VkSurfaceFormatKHR));
	vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan.physicalDevice, Vulkan.presentationSurface, &Vulkan.formatCount, &Vulkan.surfaceFormats[0]);
	// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
	// the surface has no preferred format. Otherwise, at least one
	// supported format will be returned
	if (Vulkan.formatCount == 1 && Vulkan.surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
		Vulkan.colorFormat = colorFormat;
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
VkSurfaceFormatKHR vkewGetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count, VkFormat colorFormat)
{
	// If the list contains only one entry with undefined format
	// it means that there are no preferred surface formats and any can be chosen
	int i;
	if ((count == 1) &&
		(surface_formats[0].format == VK_FORMAT_UNDEFINED)) {
		VkSurfaceFormatKHR ret;
		ret.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		ret.format = colorFormat;
		return ret;
	}
	// Check if list contains most widely used R8 G8 B8 A8 format
	// with nonlinear color space
	for (i = 0; i < count; i++) {
		if (surface_formats[i].format == colorFormat) {
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

/// <summary>
/// Release swap chain
/// </summary>
/// <param name=""></param>
void vkewReleaseSwapChain(void)
{
	if (Vulkan.swapChainParams.Value!= VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(Vulkan.i.device, Vulkan.swapChainParams.Value, NULL);
		Vulkan.swapChainParams.Value = VK_NULL_HANDLE;
	}
}


/// <summary>
/// Get Swap Chain
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VkSwapchainKHR vkewGetSwapChain(void)
{
	return Vulkan.swapChainParams.Value;
}

/// <summary>
/// Get Swap chain extend 2D
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VkExtent2D vkewGetSwapChainExtent2D(void)
{
	return Vulkan.swapChainParams.Extent;
}

/// <summary>
/// Get swap chain count
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int vkewGetSwapChainCount(void)
{
	return Vulkan.swapChainParams.ImagesCount;
}

/// <summary>
/// Get Graphics Queue
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VkQueue vkewGetGraphicsQueue(void)
{
	return Vulkan.queue;
}

/// <summary>
/// Get Transfer Queue
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VkQueue vkewGetTransferQueue(void)
{
	return Vulkan.transfertQueue;
}

/// <summary>
/// Get Graphics Queue Family Index
/// </summary>
/// <param name=""></param>
/// <returns></returns>

uint32_t vkewGetGraphicsQueueFamilyIndex(void)
{
	return Vulkan.graphicsQueueFamilyIndex;
}

/// <summary>
/// Get Transfert queue family index
/// </summary>
/// <param name=""></param>
/// <returns></returns>
uint32_t vkewGetTransfertQueueFamilyIndex(void)
{
	return Vulkan.transfertQueueFamilyIndex;
}

/// <summary>
/// Get device
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VkDevice vkewGetDevice(void)
{
	return Vulkan.i.device;
}

/// <summary>
/// Create swap chain view
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VkBool32 vkewCreateSwapChainImageViews(void) {
	for (int i = 0; i < Vulkan.swapChainParams.ImagesCount; ++i) {
		VkImageViewCreateInfo image_view_create_info = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   // VkStructureType                sType
			NULL,                                    // const void                    *pNext
			0,                                          // VkImageViewCreateFlags         flags
			Vulkan.swapChainParams.Images[i].Value,          // VkImage                        image
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


		if (vkCreateImageView(Vulkan.i.device, &image_view_create_info, NULL, &Vulkan.swapChainParams.Images[i].View) != VK_SUCCESS) {
			return VK_FALSE;
		}
	}
	return VK_TRUE;
}

// Create swap chain
// @param : vsync : enable vertical sync, if false, will try to use VK_PRESENT_MODE_IMMEDIATE_KHR
// @param : exclusive : enable exclusive mode (TODO: Not implemented, see VK_KHR_display_swapchain).
// https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/master/appendices/VK_KHR_display_swapchain.txt
VkResult vkewCreateSwapChain(void* platformWindow, int vsync, VkExtent2D desired_extent, int full_screen_exclusive, VkFormat colorFormat)
{
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
	VkSurfaceFormatKHR            desired_format = vkewGetSwapChainFormat(surface_formats, formats_count, colorFormat);
	if (desired_extent.width == 0 && desired_extent.height == 0)
	{
		desired_extent = vkewGetSwapChainExtent(&surface_capabilities);
	}
	VkImageUsageFlags             desired_usage = vkewGetSwapChainUsageFlags(&surface_capabilities);
	VkSurfaceTransformFlagBitsKHR desired_transform = vkewGetSwapChainTransform(&surface_capabilities);
	VkPresentModeKHR              desired_present_mode = vkewGetSwapChainPresentMode(present_modes, present_modes_count, vsync);
	VkSwapchainKHR                old_swap_chain = Vulkan.swapChainParams.Value;
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
	// Enable transfer source on swap chain images if supported
	if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
		swap_chain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}

	// Enable transfer destination on swap chain images if supported
	if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
		swap_chain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	Vulkan.swapChainParams.Value = VK_NULL_HANDLE;
#ifdef VK_EXT_full_screen_exclusive
	if (full_screen_exclusive && VKEW_EXT_full_screen_exclusive)
	{
		/* 
		This extension allows applications to set the policy for swapchain creation and presentation mechanisms relating to full-screen access. Implementations may be able to acquire exclusive access to a particular display for an application window that covers the whole screen. This can increase performance on some systems by bypassing composition, however it can also result in disruptive or expensive transitions in the underlying windowing system when a change occurs.
		Applications can choose between explicitly disallowing or allowing this behavior, letting the implementation decide, or managing this mode of operation directly using the new vkAcquireFullScreenExclusiveModeEXT and vkReleaseFullScreenExclusiveModeEXT commands.
		*/
		// https://gpuopen.com/using-amd-freesync-premium-pro-hdr-code-samples/
		Vulkan.surfaceFullScreenExclusiveWin32InfoEXT.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
		Vulkan.surfaceFullScreenExclusiveInfoEXT.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;

		Vulkan.surfaceFullScreenExclusiveWin32InfoEXT.hmonitor = MonitorFromWindow((HWND)platformWindow, MONITOR_DEFAULTTOPRIMARY);		
		Vulkan.surfaceFullScreenExclusiveInfoEXT.fullScreenExclusive = VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT;

		swap_chain_create_info.pNext = &Vulkan.surfaceFullScreenExclusiveInfoEXT;
		Vulkan.surfaceFullScreenExclusiveInfoEXT.pNext = &Vulkan.surfaceFullScreenExclusiveWin32InfoEXT;
		Vulkan.surfaceFullScreenExclusiveWin32InfoEXT.pNext = NULL;

		Vulkan.surfaceCapabilities2KHR.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
		Vulkan.surfaceCapabilities2KHR.pNext = NULL;

		Vulkan.physicalDeviceSurfaceInfo2KHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2_KHR;
		Vulkan.physicalDeviceSurfaceInfo2KHR.pNext = &Vulkan.surfaceFullScreenExclusiveInfoEXT;
		Vulkan.physicalDeviceSurfaceInfo2KHR.surface = Vulkan.presentationSurface;

		Vulkan.supportsExclusiveFullscreen = vkGetPhysicalDeviceSurfaceCapabilities2KHR(Vulkan.physicalDevice, &Vulkan.physicalDeviceSurfaceInfo2KHR, &Vulkan.surfaceCapabilities2KHR) == VK_SUCCESS;
	}
	if (VK_CHECK(vkCreateSwapchainKHR(Vulkan.i.device, &swap_chain_create_info, NULL, &Vulkan.swapChainParams.Value)) != VK_SUCCESS) {
		// Try without exclusive fullscreen.
		Vulkan.supportsExclusiveFullscreen = 0;
		swap_chain_create_info.pNext = NULL;
	}
	
#endif
	if (Vulkan.swapChainParams.Value == VK_NULL_HANDLE)
		if (VK_CHECK(vkCreateSwapchainKHR(Vulkan.i.device, &swap_chain_create_info, NULL, &Vulkan.swapChainParams.Value)) != VK_SUCCESS) {
			return VK_ERROR_INITIALIZATION_FAILED;
		}
	if (old_swap_chain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(Vulkan.i.device, old_swap_chain, NULL);
	}
	Vulkan.swapChainParams.Format = desired_format.format;
	uint32_t image_count = 0;
	if ((vkGetSwapchainImagesKHR(Vulkan.i.device, Vulkan.swapChainParams.Value, &image_count, NULL) != VK_SUCCESS) ||
		(image_count == 0)) {
		return VK_FALSE;
	}
	Vulkan.swapChainParams.ImagesCount = image_count;
	Vulkan.swapChainParams.Images = (struct ImageParameters*)realloc(Vulkan.swapChainParams.Images, sizeof(struct ImageParameters) * image_count);
	memset(Vulkan.swapChainParams.Images, 0, sizeof(struct ImageParameters) * image_count);
	VkImage* images = (VkImage*)calloc(image_count, sizeof(VkImage));
	if (vkGetSwapchainImagesKHR(Vulkan.i.device, Vulkan.swapChainParams.Value, &image_count, &images[0]) != VK_SUCCESS) {
		return VK_FALSE;
	}
	for (int i = 0; i < Vulkan.swapChainParams.ImagesCount; ++i) {
		Vulkan.swapChainParams.Images[i].Value = images[i];
	}

	free(images);
	/*
	if (full_screen_exclusive && VKEW_EXT_full_screen_exclusive)
	{
		auto status = vkGetPhysicalDeviceSurfacePresentModes2EXT(vkewGetPhysicalDevice(), &Vulkan.physicalDeviceSurfaceInfo2KHR, &Vulkan.numPresentationModes, NULL);
		Vulkan.presentationModes = (VkPresentModeKHR*)calloc(Vulkan.numPresentationModes, sizeof(VkPresentModeKHR));
		status = vkGetPhysicalDeviceSurfacePresentModes2EXT(vkewGetPhysicalDevice(), &Vulkan.physicalDeviceSurfaceInfo2KHR, &Vulkan.numPresentationModes, Vulkan.presentationModes);
	}
	*/
	Vulkan.swapChainParams.Extent = desired_extent;
	vkewCreateSwapChainImageViews();
	return VK_SUCCESS;
}

/// <summary>
/// Get presentation surface
/// </summary>
/// <returns></returns>
VkSurfaceKHR vkewGetPresentationSurface()
{
	return Vulkan.presentationSurface;
}

/// <summary>
/// Get physical device
/// </summary>
/// <returns></returns>
VkPhysicalDevice vkewGetPhysicalDevice()
{
	return Vulkan.physicalDevice;
}




/// <summary>
/// Get Queue Node Index
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int vkewGetQueueNodeIndex(void)
{
	return Vulkan.queueNodeIndex;
}

/// <summary>
/// Get Transfert queue index
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int vkewGetQueueTransfertIndex(void)
{
	return Vulkan.queueTransfertIndex;
}

/// <summary>
/// Get swap chain imagfe
/// </summary>
/// <param name="i"></param>
/// <returns></returns>
VkImage vkewGetSwapChainImage(int i)
{
	return Vulkan.swapChainParams.Images[i].Value;
}

/// <summary>
/// Get Swap Chain Image View 
/// </summary>
/// <param name="i"></param>
/// <returns></returns>
VkImageView vkewGetSwapChainImageView(int i)
{
	return Vulkan.swapChainParams.Images[i].View;
}
