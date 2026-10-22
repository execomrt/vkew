# Vulkan Extension Wrangler

VKEW is a small, dependency-free C library that sits between your application and the Vulkan loader.
It loads the Vulkan entry points, creates the `VkInstance` / `VkDevice`, and detects and enables optional
extensions together with their feature structs, so application code can simply test a `VKEW_<extension>` flag.

## 🎯 Goal

- **One call to a working Vulkan device**: `vkewInit()` + `vkewCreateDevice()` handle instance, layers, physical-device
  selection, queue families and extension/feature enablement.
- **Track the newest Vulkan extensions** (ray tracing, mesh shading, descriptor buffer / heap, device fault, ...) without
  every project re-writing the same loader and feature-chain boilerplate.
- **Stay small and portable**: plain C, no external libraries. Windows, Linux, Android and macOS (MoltenVK).
- **Opt-in for anything risky**: heavyweight or behavior-changing features are enabled through `VKEWSettings`, never implicitly.
- **Debug friendly**: validation, logging, error hooks and enum-to-string helpers are included.

See [CHANGELOG.md](CHANGELOG.md) for what changed.

## 🚀 Quick start

```c
VKEWSettings settings = {0};
settings.stSize = sizeof(VKEWSettings);
settings.pApplicationName = "MyApp";
settings.apiVersion = VK_MAKE_VERSION(1, 2, 0);
settings.enableValidation = 1;

if (vkewInit(&settings) != VK_SUCCESS) { /* handle error */ }
vkewCreateDevice(surface, 0);
if (VKEW_KHR_timeline_semaphore) { /* safe to use vkWaitSemaphoresKHR */ }
vkewDestroy();
```

### Building the demo

A windowed triangle demo (swapchain, resize handling) lives in [`tests/`](tests). Use the presets in `CMakePresets.json`:

```
cmake --preset msvc
cmake --build --preset msvc-debug
```

(or `ninja-debug` / `ninja-release`). On Linux and macOS use `linux-debug|release` / `macos-debug|release`
(`cmake --preset linux-debug && cmake --build --preset linux-debug`); these build the library only until the demo is ported. Run the demo with `--validation` / `--no-validation` to force the validation layer.

### Device lost sample

`vkew_devicelost` (Windows, built with the demo) deliberately faults the GPU and reads the result back through
`VK_KHR_device_fault`, so you can see what a given driver reports: fault addresses, fault type, vendor codes and the
vendor crash-dump blob. It is also a template for a crash reporter (`tests/vkDeviceFault.h` is a reusable query/format helper).

```
vkew_devicelost [--mode pagefault|hang|none] [--no-fault] [--validation]
```

Reports are written to `vkewDeviceFault-<pid>.txt` (and `.bin` for the vendor blob). Requires `glslc` to build the compute shader
and Vulkan headers with `VK_KHR_device_fault`.

### `VKEWSettings` options

| Field | Effect |
| --- | --- |
| `enableValidation` | Enables `VK_LAYER_KHRONOS_validation` and debug messenger. |
| `enableRaytracing` | Enables acceleration structure, ray tracing pipeline, ray query and their features. |
| `enableMeshShading` | Enables `VK_EXT_mesh_shader`. |
| `enableDynamicRendering` | Enables `VK_KHR_dynamic_rendering`. |
| `enableDescriptorHeap` | Enables `VK_EXT_descriptor_heap` plus `VK_KHR_device_address_commands`, `VK_KHR_shader_untyped_pointers`, `VK_KHR_unified_image_layouts`. |
| `disableDescriptorBuffer` | Skips `VK_EXT_descriptor_buffer` even when available. |
| `enablePrimitiveRestart` | Enables the primitive-restart dynamic-state feature. |
| `enableDeviceFault` | Enables `VK_KHR_device_fault` (post-mortem GPU fault reports, query-only). |

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
| `VK_EXT_pageable_device_local_memory` | Let the OS page device-local memory under pressure. |
| `VK_EXT_mutable_descriptor_type` | Mutable descriptor types (chained after descriptor buffer). |
| `VK_EXT_descriptor_heap` | Descriptor heap model (opt-in: `enableDescriptorHeap`). |
| `VK_KHR_device_address_commands` | Commands taking device addresses instead of buffer handles (opt-in with descriptor heap). |
| `VK_KHR_shader_untyped_pointers` | Untyped pointers in SPIR-V (opt-in with descriptor heap). |
| `VK_KHR_unified_image_layouts` | Unified image layouts (opt-in with descriptor heap). |

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
| `VK_NV_ray_tracing_linear_swept_spheres` | Linear swept spheres primitive (NVIDIA). |

#### Mesh shading (optional)

| Extension | Purpose |
| --- | --- |
| `VK_EXT_mesh_shader` | Task/mesh shaders and `vkCmdDrawMeshTasks*EXT` (opt-in: `enableMeshShading`). |

#### Shader and debug utilities

| Extension | Purpose |
| --- | --- |
| `VK_EXT_shader_demote_to_helper_invocation` | Shader demote-to-helper-invocation. |
| `VK_KHR_shader_clock` | Shader clock access. |
| `VK_KHR_shader_float16_int8` | Float16 / Int8 arithmetic. |
| `VK_EXT_robustness2` | Additional robustness guarantees (driver dependent). |
| `VK_EXT_conditional_rendering` | Predicated rendering/dispatch. |
| `VK_EXT_hdr_metadata` | `vkSetHdrMetadataEXT` for HDR swapchains. |
| `VK_KHR_device_fault` | Post-mortem GPU fault reports (opt-in: `enableDeviceFault`). |
| `VK_NV_device_diagnostic_checkpoints` / `VK_NV_device_diagnostics_config` | Nsight Aftermath support (NVIDIA, only when built with `GFX_HAS_AFTERMATH`). |

