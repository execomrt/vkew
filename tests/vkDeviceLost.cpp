/// @file vkDeviceLost.cpp
/// @brief Sample: provoke a GPU fault on purpose and read it back through VK_KHR_device_fault.
///
/// It drives vkew directly: init, create a device with VKEWSettings::enableDeviceFault, submit a compute dispatch
/// that faults, then query the fault reports. The only window is a hidden one, because vkewCreateDevice picks a
/// queue family that can present. Use it to check how a driver / GPU reports faults, or as a template for a crash
/// reporter. Windows only for now.
///
/// Usage:  vkew_devicelost [--mode pagefault|hang|none] [--no-fault] [--validation]
///
///  --mode pagefault  (default) Compute shader writes through a device address that is not mapped.
///  --mode hang       Compute shader spins forever on a valid buffer; the OS watchdog (TDR) removes the device.
///  --mode none       Submit a harmless dispatch, then still query the fault reports (recovered faults).
///  --no-fault        Leave VK_KHR_device_fault disabled, to compare against a run without it.
///  --validation      Enable the validation layer (it will report the intentional invalid access).
/// The same options are read from VKEW_CRASH_MODE=<mode> and VKEW_DEVICE_FAULT=0 when no argument is given.
///
/// What it reports
///  - The VkResult of the queue submit and of the fence wait (VK_ERROR_DEVICE_LOST expected).
///  - vkGetDeviceFaultReportsKHR: flags, fault/instruction address and type, vendor description and codes.
///  - vkGetDeviceFaultDebugInfoKHR (after loss, if deviceFaultVendorBinary): saved to vkewDeviceFault-<pid>.bin.
///  The text report is also saved to vkewDeviceFault-<pid>.txt.
///
/// Exit code: 0 when the device was lost (or mode none), 1 when a fault was requested but the GPU tolerated it,
/// 2 on setup failure.
///
/// Resources are intentionally never destroyed: the GPU may still be running the faulting work or the device is gone.
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif
#include "vkew.h"
#include "vkDeviceFault.h"

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#ifndef VK_KHR_device_fault
#error "vkDeviceLost needs Vulkan headers with VK_KHR_device_fault (SDK 1.4.347 or newer)"
#endif

namespace
{
enum class CrashMode
{
    None,
    PageFault,
    Hang,
};

struct Options
{
    CrashMode mode = CrashMode::PageFault;
    bool enableFault = true;
    bool validation = false;
};

/// Matches the push constant block of shaders/device_lost_crash.comp.
struct PushData
{
    uint64_t target;
    uint32_t spin;
    uint32_t pad;
};

/// Everything the dispatch needs. Never destroyed on purpose, see the file comment.
struct CrashContext
{
    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
    uint32_t queueFamily = 0;
    VkPipelineLayout layout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;
};

std::string g_report; // everything printed through Out(), saved to the .txt at the end

void Out(const char *fmt, ...)
{
    char line[1024];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(line, sizeof(line), fmt, args);
    va_end(args);
    std::fputs(line, stdout);
    std::fputc('\n', stdout);
    g_report += line;
    g_report += '\n';
}

void VkewLog(VKEWMessageLevel level, const char *fmt, ...)
{
    char line[1024];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(line, sizeof(line), fmt, args);
    va_end(args);
    std::fprintf(stderr, "[vkew %s] %s\n", level == VKEW_MESSAGE_ERROR ? "error" : level == VKEW_MESSAGE_WARNING ? "warn" : "info", line);
}

bool ParseMode(const char *value, CrashMode &mode)
{
    if (!_stricmp(value, "pagefault"))
        mode = CrashMode::PageFault;
    else if (!_stricmp(value, "hang"))
        mode = CrashMode::Hang;
    else if (!_stricmp(value, "none"))
        mode = CrashMode::None;
    else
        return false;
    return true;
}

const char *ModeName(CrashMode mode)
{
    return mode == CrashMode::None ? "none" : mode == CrashMode::Hang ? "hang" : "pagefault";
}

bool ParseOptions(int argc, char **argv, Options &options)
{
    if (const char *env = std::getenv("VKEW_CRASH_MODE"))
        ParseMode(env, options.mode);
    if (const char *env = std::getenv("VKEW_DEVICE_FAULT"))
        options.enableFault = env[0] != '0';

    for (int i = 1; i < argc; ++i)
    {
        if (!std::strcmp(argv[i], "--mode") && i + 1 < argc)
        {
            if (!ParseMode(argv[++i], options.mode))
            {
                std::fprintf(stderr, "unknown mode '%s'\n", argv[i]);
                return false;
            }
        }
        else if (!std::strcmp(argv[i], "--no-fault"))
            options.enableFault = false;
        else if (!std::strcmp(argv[i], "--validation"))
            options.validation = true;
        else
        {
            std::fprintf(stderr, "usage: vkew_devicelost [--mode pagefault|hang|none] [--no-fault] [--validation]\n");
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK HiddenWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    return DefWindowProcA(hwnd, msg, wp, lp);
}

// vkewCreateDevice wants a surface to find a present-capable queue family; a hidden window is enough.
VkSurfaceKHR CreateHiddenSurface()
{
    WNDCLASSA wc{};
    wc.lpfnWndProc = HiddenWndProc;
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.lpszClassName = "vkewDeviceLost";
    RegisterClassA(&wc);
    HWND hwnd = CreateWindowExA(0, wc.lpszClassName, "vkewDeviceLost", WS_OVERLAPPEDWINDOW, 0, 0, 64, 64, nullptr, nullptr, wc.hInstance, nullptr);
    if (!hwnd)
        return VK_NULL_HANDLE;
    VkWin32SurfaceCreateInfoKHR info{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
    info.hinstance = wc.hInstance;
    info.hwnd = hwnd;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (vkCreateWin32SurfaceKHR(vkewGetInstance(), &info, vkewGetAllocationCallbacks(), &surface) != VK_SUCCESS)
        return VK_NULL_HANDLE;
    return surface;
}

// SPIR-V is compiled next to the executable by CMake (shaders/spirv/device_lost_crash.spv).
std::vector<uint32_t> LoadCrashShader()
{
    char path[MAX_PATH];
    const DWORD n = GetModuleFileNameA(nullptr, path, MAX_PATH);
    std::string file(path, n);
    file.erase(file.find_last_of("\\/") + 1);
    file += "shaders\\spirv\\device_lost_crash.spv";

    std::vector<uint32_t> words;
    if (FILE *f = std::fopen(file.c_str(), "rb"))
    {
        std::fseek(f, 0, SEEK_END);
        const long size = std::ftell(f);
        std::fseek(f, 0, SEEK_SET);
        words.resize(static_cast<size_t>(size) / sizeof(uint32_t));
        if (std::fread(words.data(), sizeof(uint32_t), words.size(), f) != words.size())
            words.clear();
        std::fclose(f);
    }
    else
    {
        std::fprintf(stderr, "cannot open %s (is glslc installed? it is needed to build the shader)\n", file.c_str());
    }
    return words;
}

bool FindMemoryType(uint32_t typeBits, VkMemoryPropertyFlags wanted, uint32_t &index)
{
    VkPhysicalDeviceMemoryProperties props{};
    vkGetPhysicalDeviceMemoryProperties(vkewGetPhysicalDevice(), &props);
    for (uint32_t i = 0; i < props.memoryTypeCount; ++i)
    {
        if ((typeBits & (1u << i)) && (props.memoryTypes[i].propertyFlags & wanted) == wanted)
        {
            index = i;
            return true;
        }
    }
    return false;
}

// 64 KiB device-local buffer with a device address, used as the valid target of the hang and none modes.
bool CreateTargetBuffer(VkDevice device, VkDeviceAddress &address)
{
    VkBufferCreateInfo bi{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    bi.size = 64 * 1024;
    bi.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
    bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkBuffer buffer = VK_NULL_HANDLE;
    if (vkCreateBuffer(device, &bi, nullptr, &buffer) != VK_SUCCESS)
        return false;
    VkMemoryRequirements req{};
    vkGetBufferMemoryRequirements(device, buffer, &req);
    uint32_t type = 0;
    if (!FindMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, type))
        return false;
    VkMemoryAllocateFlagsInfo flags{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO };
    flags.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
    VkMemoryAllocateInfo ai{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, &flags };
    ai.allocationSize = req.size;
    ai.memoryTypeIndex = type;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    if (vkAllocateMemory(device, &ai, nullptr, &memory) != VK_SUCCESS || vkBindBufferMemory(device, buffer, memory, 0) != VK_SUCCESS)
        return false;
    VkBufferDeviceAddressInfo addr{ VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
    addr.buffer = buffer;
    address = vkGetBufferDeviceAddress(device, &addr);
    return address != 0;
}

bool CreateCrashPipeline(CrashContext &ctx)
{
    const std::vector<uint32_t> code = LoadCrashShader();
    if (code.empty())
        return false;
    VkShaderModuleCreateInfo sm{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    sm.codeSize = code.size() * sizeof(uint32_t);
    sm.pCode = code.data();
    VkShaderModule module = VK_NULL_HANDLE;
    if (vkCreateShaderModule(ctx.device, &sm, nullptr, &module) != VK_SUCCESS)
        return false;

    VkPushConstantRange range{ VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(PushData) };
    VkPipelineLayoutCreateInfo li{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
    li.pushConstantRangeCount = 1;
    li.pPushConstantRanges = &range;
    if (vkCreatePipelineLayout(ctx.device, &li, nullptr, &ctx.layout) != VK_SUCCESS)
        return false;

    VkComputePipelineCreateInfo pi{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
    pi.stage = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_COMPUTE_BIT, module, "main", nullptr };
    pi.layout = ctx.layout;
    return vkCreateComputePipelines(ctx.device, VK_NULL_HANDLE, 1, &pi, nullptr, &ctx.pipeline) == VK_SUCCESS;
}

// Initializes vkew, creates the device and the crash pipeline. Returns false (message already printed) on failure.
bool Setup(const Options &options, CrashContext &ctx)
{
    VKEWSettings settings{};
    settings.stSize = sizeof(settings);
    settings.pApplicationName = "vkewDeviceLost";
    settings.pEngineName = "vkew";
    settings.apiVersion = VK_MAKE_VERSION(1, 3, 0);
    settings.enableValidation = options.validation ? 1 : 0;
    settings.enableDeviceFault = options.enableFault ? 1 : 0;
    settings.pfnLog = VkewLog;
    if (vkewInit(&settings) != VK_SUCCESS)
    {
        std::fprintf(stderr, "vkewInit failed\n");
        return false;
    }

    VkSurfaceKHR surface = CreateHiddenSurface();
    if (surface == VK_NULL_HANDLE || vkewCreateDevice(surface, -1) != VK_SUCCESS)
    {
        std::fprintf(stderr, "device creation failed\n");
        return false;
    }
    ctx.device = vkewGetDevice();
    ctx.queueFamily = vkewGetGraphicsQueueFamilyIndex();
    vkGetDeviceQueue(ctx.device, ctx.queueFamily, 0, &ctx.queue);

    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(vkewGetPhysicalDevice(), &props);
    Out("GPU: %s (vendor 0x%04X, driver 0x%X)", props.deviceName, props.vendorID, props.driverVersion);
    Out("vkDeviceLost: crash mode '%s', VK_KHR_device_fault %s (requested %s)", ModeName(options.mode),
        VKEW_KHR_device_fault ? "enabled" : "NOT enabled", options.enableFault ? "yes" : "no");

    if (!CreateCrashPipeline(ctx))
    {
        std::fprintf(stderr, "crash pipeline creation failed\n");
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------
// Crash
// ---------------------------------------------------------------------------------------------------------------

// Fills the push constants for the chosen mode. Returns false (message already printed) on failure.
bool PreparePush(const CrashContext &ctx, CrashMode mode, PushData &push)
{
    push = {};
    push.spin = mode == CrashMode::Hang ? 1u : 0u;
    if (mode == CrashMode::PageFault)
    {
        // Far outside anything this process allocated; faults reliably on current drivers.
        push.target = 0x000000DEAD000000ull;
        Out("dispatching a compute write to unmapped GPU VA 0x%016llX", static_cast<unsigned long long>(push.target));
        return true;
    }

    VkDeviceAddress address = 0;
    if (!CreateTargetBuffer(ctx.device, address))
    {
        std::fprintf(stderr, "target buffer creation failed (bufferDeviceAddress unavailable?)\n");
        return false;
    }
    push.target = address;
    Out(mode == CrashMode::Hang ? "dispatching an endless compute loop (TDR in ~2 s)" : "dispatching a harmless compute write");
    return true;
}

// Records and submits the dispatch, then waits for it. Returns true when the device was lost.
bool SubmitAndWait(const CrashContext &ctx, const PushData &push)
{
    VkCommandPoolCreateInfo pci{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    pci.queueFamilyIndex = ctx.queueFamily;
    VkCommandPool pool = VK_NULL_HANDLE;
    vkCreateCommandPool(ctx.device, &pci, nullptr, &pool);
    VkCommandBufferAllocateInfo cai{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    cai.commandPool = pool;
    cai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cai.commandBufferCount = 1;
    VkCommandBuffer cmd = VK_NULL_HANDLE;
    vkAllocateCommandBuffers(ctx.device, &cai, &cmd);

    VkCommandBufferBeginInfo bi{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &bi);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, ctx.pipeline);
    vkCmdPushConstants(cmd, ctx.layout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(push), &push);
    vkCmdDispatch(cmd, 1, 1, 1);
    vkEndCommandBuffer(cmd);

    VkFenceCreateInfo fi{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    VkFence fence = VK_NULL_HANDLE;
    vkCreateFence(ctx.device, &fi, nullptr, &fence);
    VkSubmitInfo si{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cmd;
    const VkResult submit = vkQueueSubmit(ctx.queue, 1, &si, fence);
    Out("vkQueueSubmit = %s", vkewVkResultToString(submit));
    const VkResult wait = submit == VK_SUCCESS ? vkWaitForFences(ctx.device, 1, &fence, VK_TRUE, 15ull * 1000000000ull) : submit;
    Out("vkWaitForFences = %s", vkewVkResultToString(wait));
    return wait == VK_ERROR_DEVICE_LOST || submit == VK_ERROR_DEVICE_LOST;
}

// ---------------------------------------------------------------------------------------------------------------
// Report
// ---------------------------------------------------------------------------------------------------------------

void SaveFile(const char *name, const void *data, size_t size)
{
    if (FILE *f = std::fopen(name, "wb"))
    {
        std::fwrite(data, 1, size, f);
        std::fclose(f);
        std::printf("wrote %s\n", name);
    }
}

// Queries and prints the fault reports. Valid after VK_ERROR_DEVICE_LOST, and (KHR) also for faults the driver
// recovered from. The vendor blob is saved next to the text report.
void ReadFaultReport(VkDevice device, bool lost)
{
    const vkewtest::DeviceFaultReport report = vkewtest::QueryDeviceFault(device, lost);
    const std::string text = vkewtest::FormatDeviceFaultReport(report);
    std::fputs(text.c_str(), stdout);
    g_report += text;
    if (!report.vendorBinary.empty())
    {
        char name[64];
        std::snprintf(name, sizeof(name), "vkewDeviceFault-%lu.bin", static_cast<unsigned long>(GetCurrentProcessId()));
        SaveFile(name, report.vendorBinary.data(), report.vendorBinary.size());
    }
}
} // namespace

int main(int argc, char **argv)
{
    Options options;
    if (!ParseOptions(argc, argv, options))
        return 2;

    CrashContext ctx;
    if (!Setup(options, ctx))
        return 2;

    PushData push;
    if (!PreparePush(ctx, options.mode, push))
        return 2;

    const bool lost = SubmitAndWait(ctx, push);
    if (lost)
        Out("DEVICE LOST");
    else if (options.mode != CrashMode::None)
        Out("The GPU did not fault (driver tolerated the access); querying reports anyway.");
    ReadFaultReport(ctx.device, lost);

    char name[64];
    std::snprintf(name, sizeof(name), "vkewDeviceFault-%lu.txt", static_cast<unsigned long>(GetCurrentProcessId()));
    SaveFile(name, g_report.data(), g_report.size());
    std::fflush(nullptr);

    // Skip static destructors and vkewDestroy: the device may be lost or the GPU still busy.
    std::_Exit(lost || options.mode == CrashMode::None ? 0 : 1);
}
