# Vulkan Extension Wrangler

This library allows initializing Vulkan and adds important extensions. It doesn't require extra libraries.
Debug services, and enum to string converters are included.

## 🔥 Core Features

### Instance and Device Management

- Creation and destruction of `VkInstance` and `VkDevice`.
- Querying available Vulkan physical devices.
- Fetching physical device properties and features.
- Support for multiple platforms: Windows, Linux (XCB/Xlib/Wayland), Android, macOS (MoltenVK).

### Function Loader

- Dynamic function lookup for Vulkan API functions (`vkGetInstanceProcAddr` / `vkGetDeviceProcAddr`).
- Handles both instance-level and device-level Vulkan functions.
- Support for `VK_NO_PROTOTYPES` to manually load Vulkan functions.

### Extension & Layer Management

- Enumerates and checks for supported Vulkan instance and device extensions.
- Enables validation layers (`VK_LAYER_KHRONOS_validation`).
- Platform-specific extensions: `VK_KHR_win32_surface`, `VK_KHR_xcb_surface`, `VK_KHR_wayland_surface`, `VK_KHR_android_surface`, etc.

### Swap Chain Management

## 🛠 Debugging & Logging Features

### Debug Output Support

- Supports `VK_EXT_debug_utils` and `VK_EXT_debug_report`.
- Debug message callbacks for validation layer errors and warnings.
- Provides function `vkewLogMessage()` for structured logging.
- Detects Vulkan API call failures with `vkewPrintResult()`.

### Error Handling

- Converts Vulkan result codes (`VkResult`) into human-readable messages.
- Logs missing extensions and validation layers.
- Offers `vkewOnErrors()` for custom error handling.

## ⚙️ Synchronization & Command Management

### Queue & Command Buffer Management

- Queries queue families (`vkewGetGraphicsQueueFamilyIndex()`).
- Retrieves and initializes command pools and command buffers.
- Loads core command submission functions (`vkQueueSubmit`, `vkQueuePresentKHR`).

### Synchronization Primitives

- Creates and manages semaphores (`vkCreateSemaphore`).
- Handles fences for GPU synchronization (`vkWaitForFences`).
- Supports timeline semaphores (`VK_KHR_timeline_semaphore`).

## 📦 Advanced Vulkan Features

### Ray Tracing Support

- Supports `VK_KHR_ray_tracing_pipeline` and `VK_KHR_acceleration_structure`.
- Provides function pointers for ray tracing operations.
- Fetches ray tracing pipeline properties and acceleration structure features.

### Pipeline & Descriptor Management

- Creates render passes, framebuffers, pipelines.
- Manages descriptor sets and pipeline layouts.

### Memory Management

- Fetches memory requirements (`vkGetBufferMemoryRequirements`).
- Supports memory mapping (`vkMapMemory`, `vkUnmapMemory`).
- Queries buffer and image device addresses (`VK_KHR_buffer_device_address`).

## 🌍 Platform-Specific Features

### Windows (`VK_USE_PLATFORM_WIN32_KHR`)

- Uses `vkCreateWin32SurfaceKHR` for surface creation.
- Provides full-screen exclusive mode (`VK_EXT_full_screen_exclusive`).

### Linux (`VK_USE_PLATFORM_XCB_KHR`, `VK_USE_PLATFORM_XLIB_KHR`, `VK_USE_PLATFORM_WAYLAND_KHR`)

- Supports `vkCreateXcbSurfaceKHR`, `vkCreateXlibSurfaceKHR`, `vkCreateWaylandSurfaceKHR`.

### Android (`VK_USE_PLATFORM_ANDROID_KHR`)

- Uses `vkCreateAndroidSurfaceKHR`.

### MacOS/iOS (MoltenVK)

- Supports `VK_MVK_macos_surface` and `VK_MVK_ios_surface`.

## 🛠 Utility Functions

### String Converters

- Converts `VkObjectType` and `VkResult` to human-readable strings.

### Feature Detection

- Checks for optional Vulkan features like `VK_KHR_synchronization2`, `VK_EXT_descriptor_indexing`, etc.
- Queries `vkewSupportsFullscreenExclusive()` for full-screen support.

## Supported Vulkan Extensions in VKEW

VKEW enumerates instance/device extensions at runtime and exposes a boolean flag per supported extension (`VKEW_<EXTENSION_NAME>`). Extensions are enabled only when available on the current driver / platform.

### Instance extensions (enabled during `vkCreateInstance`)

| Extension | Purpose |
| --- | --- |
| `VK_KHR_surface` | Base surface extension required for presentation. |
| `VK_KHR_win32_surface` / `VK_KHR_xlib_surface` / `VK_KHR_xcb_surface` / `VK_KHR_wayland_surface` / `VK_KHR_android_surface` / `VK_MVK_macos_surface` / `VK_MVK_ios_surface` | Platform surface integration (selected by compile-time platform macro). |
| `VK_KHR_get_physical_device_properties2` | Required for querying extended physical-device properties/features. |
| `VK_KHR_get_surface_capabilities2` | Extended surface capability queries (used when present). |
| `VK_KHR_driver_properties` | Extended driver/vendor properties (used when present). |
| `VK_EXT_debug_utils` | Modern debug messenger (enabled when validation is enabled). |
| `VK_EXT_debug_report` | Legacy debug callbacks (enabled when validation is enabled and available). |
| `VK_EXT_debug_marker` | Debug object markers/names (if available). |

### Device extensions

`VK_KHR_swapchain` is required. The following optional extensions are detected and exposed as flags when available:

#### Core / maintenance

| Extension | Purpose |
| --- | --- |
| `VK_KHR_maintenance1` | Misc. API/behavior fixes. |
| `VK_KHR_maintenance2` | Misc. API/behavior fixes. |
| `VK_KHR_maintenance3` | Misc. API/behavior fixes (descriptor-related). |
| `VK_KHR_maintenance4` | Limits and feature refinements. |
| `VK_KHR_maintenance5` | Additional limits/behavior refinements. |
| `VK_KHR_portability_enumeration` | Portability-enumeration support (notably relevant on some platforms). |
| `VK_KHR_device_group` | Multi-device / device-group support. |
| `VK_KHR_multiview` | Multiview rendering support. |

#### Swapchain, presentation, and formats

| Extension | Purpose |
| --- | --- |
| `VK_KHR_swapchain` | Swapchain creation and presentation (**required**). |
| `VK_KHR_display` | Display / direct-to-display support (when exposed by the driver). |
| `VK_EXT_swapchain_colorspace` | Additional swapchain color spaces (HDR / wide-gamut related). |
| `VK_KHR_image_format_list` | Image format lists (commonly used with swapchains and views). |
| `VK_KHR_create_renderpass2` | RenderPass2 creation API. |
| `VK_KHR_dynamic_rendering` | Dynamic rendering (rendering without predefined render passes). |
| `VK_KHR_present_id` | Present IDs for tracking presentation. |
| `VK_KHR_present_wait` | Present-wait support. |

#### Synchronization and scheduling

| Extension | Purpose |
| --- | --- |
| `VK_KHR_synchronization2` | Synchronization2 structures and APIs. |
| `VK_KHR_timeline_semaphore` | Timeline semaphores. |

#### Descriptors, resource access, and memory

| Extension | Purpose |
| --- | --- |
| `VK_EXT_descriptor_indexing` | Descriptor indexing / bindless-style patterns. |
| `VK_EXT_descriptor_buffer` | Descriptor buffer model. |
| `VK_KHR_push_descriptor` | Push descriptors. |
| `VK_KHR_buffer_device_address` | Buffer device address support. |
| `VK_KHR_deferred_host_operations` | Host-side deferred operations (often used by RT features). |
| `VK_KHR_separate_depth_stencil_layouts` | Separate depth/stencil layouts for transitions. |
| `VK_EXT_memory_priority` | Memory priority (when supported by the driver). |

#### Pipeline and dynamic state

| Extension | Purpose |
| --- | --- |
| `VK_EXT_extended_dynamic_state` | Extended dynamic state (v1). |
| `VK_EXT_extended_dynamic_state2` | Extended dynamic state (v2). |
| `VK_EXT_extended_dynamic_state3` | Extended dynamic state (v3). |
| `VK_EXT_pipeline_creation_cache_control` | Pipeline creation cache-control hints. |
| `VK_EXT_subgroup_size_control` | Subgroup size controls. |

#### Ray tracing (optional)

| Extension | Purpose |
| --- | --- |
| `VK_KHR_acceleration_structure` | Acceleration structures. |
| `VK_KHR_ray_tracing_pipeline` | Ray tracing pipeline. |
| `VK_KHR_ray_query` | Ray queries from any shader stage. |

#### Shader and debug utilities

| Extension | Purpose |
| --- | --- |
| `VK_EXT_shader_demote_to_helper_invocation` | Shader demote-to-helper-invocation. |
| `VK_KHR_shader_clock` | Shader clock access. |
| `VK_KHR_shader_float16_int8` | Float16 / Int8 arithmetic. |
| `VK_EXT_robustness2` | Additional robustness guarantees (driver dependent). |

