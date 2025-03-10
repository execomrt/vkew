# Vulkan Extension Wrangler
This library allow to initialize Vulkan and adds some extensions. It doesn't requires extra library.

🔥 Core Features
Instance and Device Management

Creation and destruction of VkInstance and VkDevice.
Querying available Vulkan physical devices.
Fetching physical device properties and features.
Support for multiple platforms: Windows, Linux (XCB/Xlib/Wayland), Android, macOS (MoltenVK).
Function Loader

Dynamic function lookup for Vulkan API functions (vkGetInstanceProcAddr / vkGetDeviceProcAddr).
Handles both instance-level and device-level Vulkan functions.
Support for VK_NO_PROTOTYPES to manually load Vulkan functions.
Extension & Layer Management

Enumerates and checks for supported Vulkan instance and device extensions.
Enables validation layers (VK_LAYER_KHRONOS_validation).
Platform-specific extensions: VK_KHR_win32_surface, VK_KHR_xcb_surface, VK_KHR_wayland_surface, VK_KHR_android_surface, etc.
Swap Chain Management

🛠 Debugging & Logging Features
Debug Output Support

Supports VK_EXT_debug_utils and VK_EXT_debug_report.
Debug message callbacks for validation layer errors and warnings.
Provides function vkewLogMessage() for structured logging.
Detects Vulkan API call failures with vkewPrintResult().
Error Handling

Converts Vulkan result codes (VkResult) into human-readable messages.
Logs missing extensions and validation layers.
Offers vkewOnErrors() for custom error handling.
⚙️ Synchronization & Command Management
Queue & Command Buffer Management

Queries queue families (vkewGetGraphicsQueueFamilyIndex()).
Retrieves and initializes command pools and command buffers.
Loads core command submission functions (vkQueueSubmit, vkQueuePresentKHR).
Synchronization Primitives

Creates and manages semaphores (vkCreateSemaphore).
Handles fences for GPU synchronization (vkWaitForFences).
Supports timeline semaphores (VK_KHR_timeline_semaphore).
📦 Advanced Vulkan Features
Ray Tracing Support

Supports VK_KHR_ray_tracing_pipeline and VK_KHR_acceleration_structure.
Provides function pointers for ray tracing operations.
Fetches ray tracing pipeline properties and acceleration structure features.
Pipeline & Descriptor Management

Creates render passes, framebuffers, pipelines.
Manages descriptor sets and pipeline layouts.
Memory Management

Fetches memory requirements (vkGetBufferMemoryRequirements).
Supports memory mapping (vkMapMemory, vkUnmapMemory).
Queries buffer and image device addresses (VK_KHR_buffer_device_address).

🌍 Platform-Specific Features
Windows (VK_USE_PLATFORM_WIN32_KHR)

Uses vkCreateWin32SurfaceKHR for surface creation.
Provides full-screen exclusive mode (VK_EXT_full_screen_exclusive).
Linux (VK_USE_PLATFORM_XCB_KHR, VK_USE_PLATFORM_XLIB_KHR, VK_USE_PLATFORM_WAYLAND_KHR)

Supports vkCreateXcbSurfaceKHR, vkCreateXlibSurfaceKHR, vkCreateWaylandSurfaceKHR.
Android (VK_USE_PLATFORM_ANDROID_KHR)

Uses vkCreateAndroidSurfaceKHR.
MacOS/iOS (MoltenVK)

Supports VK_MVK_macos_surface and VK_MVK_ios_surface.
🛠 Utility Functions
String Converters

Converts VkObjectType and VkResult to human-readable strings.
Feature Detection

Checks for optional Vulkan features like VK_KHR_synchronization2, VK_EXT_descriptor_indexing, etc.
Queries vkewSupportsFullscreenExclusive() for full-screen support.

Supported Vulkan Extensions in VKEW
The Vulkan Extension Wrangler (VKEW) automatically detects and enables supported extensions. Each extension has a corresponding boolean flag (VKEW_<EXTENSION_NAME>) that indicates whether the extension is available.

✅ Core Vulkan Extensions
	Extension	Feature
	VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2	Required for querying additional device properties.
	VK_KHR_DRIVER_PROPERTIES	Provides detailed information about the Vulkan driver.
	VK_KHR_MAINTENANCE_1	Fixes descriptor set binding and adds other minor improvements.
	VK_KHR_MAINTENANCE_2	Standardizes image block matching and other optimizations.
	VK_KHR_MAINTENANCE_3	Introduces descriptor indexing.
	VK_KHR_MAINTENANCE_4	Extends limits and improves resource creation.
	VK_KHR_MAINTENANCE_5	Enables dynamic rendering, improves image transitions.
🖼 Swap Chain & Rendering Extensions
	Extension	Feature
	VK_KHR_SWAPCHAIN	Enables swap chains for rendering to window surfaces.
	VK_KHR_IMAGE_FORMAT_LIST	Optimizes swap chain image formats.
	VK_KHR_CREATE_RENDERPASS_2	Improves render pass creation.
	VK_KHR_DYNAMIC_RENDERING	Allows rendering without predefined render passes.
	VK_KHR_DEPTH_STENCIL_RESOLVE	Improves multisampling depth-stencil resolve.
🚀 Synchronization & Performance Extensions
	Extension	Feature
	VK_KHR_SYNCHRONIZATION_2	Provides enhanced synchronization primitives.
	VK_KHR_TIMELINE_SEMAPHORE	Enables timeline semaphores for better GPU-CPU synchronization.
	VK_KHR_SHADER_CLOCK	Allows shaders to access a high-resolution clock.
	VK_KHR_SHADER_FLOAT16_INT8	Enables float16 and int8 arithmetic in shaders.
🛠 Advanced Features & Optimization
	Extension	Feature
	VK_KHR_BUFFER_DEVICE_ADDRESS	Enables direct access to buffer device addresses.
	VK_EXT_DESCRIPTOR_INDEXING	Supports bindless resources and large descriptor sets.
	VK_KHR_PUSH_DESCRIPTOR	Allows directly pushing descriptors to shaders.
	VK_KHR_DEFERRED_HOST_OPERATIONS	Optimizes host-side Vulkan operations.
🔦 Debugging & Developer Tools
	Extension	Feature
	VK_EXT_DEBUG_MARKER	Allows naming Vulkan objects (useful for debugging).
	VK_EXT_ROBUSTNESS_2	Adds stricter memory safety checks for buffers/images.
🔦 Ray Tracing Support (Optional)
	Extension	Feature
	VK_KHR_RAY_TRACING_PIPELINE	Enables ray tracing pipeline support.
	VK_KHR_RAY_QUERY	Allows ray tracing from any shader stage.
	VK_KHR_ACCELERATION_STRUCTURE	Provides ray tracing acceleration structure support.
🎮 Platform-Specific Extensions
	Extension	Feature
	VK_EXT_FULL_SCREEN_EXCLUSIVE	Enables exclusive full-screen support (Windows only).
