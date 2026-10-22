# Changelog

## Unreleased

### Library (`src/vkew.c`, `src/vkew.h`)

New `VKEWSettings` options (all opt-in): `enableMeshShading`, `enableDescriptorHeap`, `enablePrimitiveRestart`,
`disableDescriptorBuffer`, `enableDeviceFault`.

New extensions detected / enabled (each exposes a `VKEW_<name>` flag):
- `VK_EXT_mesh_shader` (mesh-task entry points moved out of the `VK_KHR_ray_query` block)
- `VK_EXT_descriptor_heap` with its function pointers, plus `VK_KHR_device_address_commands`,
  `VK_KHR_shader_untyped_pointers`, `VK_KHR_unified_image_layouts`
- `VK_EXT_mutable_descriptor_type`, `VK_EXT_conditional_rendering`, `VK_EXT_hdr_metadata`,
  `VK_EXT_pageable_device_local_memory`
- `VK_KHR_device_fault` (`VKEW_KHR_device_fault_vendor_binary` reports the vendor-binary feature)
- `VK_NV_ray_tracing_linear_swept_spheres`
- `VK_NV_device_diagnostic_checkpoints` / `VK_NV_device_diagnostics_config` (Nsight Aftermath, only with `GFX_HAS_AFTERMATH`)

New / now-loaded entry points: `vkCmdFillBuffer`, `vkCmdUpdateBuffer`, `vkGetPhysicalDeviceImageFormatProperties`,
`vkSetHdrMetadataEXT`, timeline semaphore functions (`vkGetSemaphoreCounterValueKHR`, `vkWaitSemaphoresKHR`,
`vkSignalSemaphoreKHR`).

Behavior changes:
- Features are queried through `vkewQueryFeatures`; a `VKEW_*` flag is only left set when the driver reports the feature
  (e.g. memory priority, pageable memory, device fault).
- Instance extension flags (`debug_utils`, `debug_report`, `swapchain_colorspace`, `get_surface_capabilities2`) are reset on init
  and set only when found; debug utils is only used when validation is enabled.
- `VK_EXT_swapchain_colorspace` is treated as an instance extension only.

- `vkew.c` now builds warning-free at MSVC `/W4` (unused parameters, function/data pointer conversions in the proc-address
  loader, signed/unsigned device index comparison).

### Demo (`tests/`)
- Resize no longer leaks the pipeline, render pass, shaders, vertex buffer and fences; they are released before recreation.
- `VK_ERROR_OUT_OF_DATE_KHR` on acquire / present triggers swapchain recreation; minimized windows are skipped.
- Fixed undefined behavior in the log callback (`va_list` reused after `vprintf`).
- Validation defaults on in Debug and off in Release; added `--no-validation`.
- Demo explicitly lists the opt-in `VKEWSettings` features.

### Samples
- New `vkew_devicelost` sample (`tests/vkDeviceLost.cpp`, `tests/vkDeviceFault.h`, `tests/shaders/device_lost_crash.comp`):
  provokes a page fault or a hang and reads the `VK_KHR_device_fault` reports and vendor binary. Windows only for now.

### Build
- Added `CMakePresets.json` (`msvc`, `ninja-debug`, `ninja-release`, `linux-*`, `macos-*`); removed `generate_vs.bat`.
- SPIR-V custom command now depends on the real shader files (`triangle_vs.vert`, `triangle_ps.frag`).

### Docs
- README: stated the project goal, added quick start, `VKEWSettings` table and the new extensions.

## Earlier history (from git log)
- 2026-01: refactored the Vulkan test infrastructure, improved resource cleanup and pipeline setup.
- 2025-09: `vkewQueryFeatures`, proper ray tracing feature enabling.
- 2025-08: `vkResetQueryPool`, ray tracing support; 2025-05: fixed `VK_KHR_acceleration_structure` / `VK_KHR_ray_tracing_pipeline`.
- 2025-04: fixed initialization layers; 2025-03: removed `l_calloc`/`l_free`, added `vkewGetAllocationCallbacks` and enum-to-string helpers.
- 2024: swapchain creation moved out of vkew into the sample, `VKEW_KHR_dynamic_rendering`, synchronization2, CMake file and demo.
