#ifdef IS_PLATFORM_WIN
#include <windows.h>
#endif

#include "vkew.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <filesystem>

#define VK_ASSERT(f) { \
    if (!(f)) { \
        std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
}

static VkBool32 useValidationLayer = VK_TRUE;
#include "vkTest.h"

static VulkanResources g_Resources;

#ifdef IS_PLATFORM_WIN
static std::filesystem::path GetExecutableDir()
{
    wchar_t pathW[MAX_PATH] = {};
    const DWORD n = GetModuleFileNameW(nullptr, pathW, MAX_PATH);
    if (n == 0 || n >= MAX_PATH)
        return std::filesystem::current_path();

    std::filesystem::path p(pathW);
    return p.parent_path();
}
#endif

void ParseCommandLineArgs(int argc, const char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--validation") == 0)
        {
            useValidationLayer = VK_TRUE;
        }
    }
}

void CreateCommandPoolVulkan()
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = vkewGetGraphicsQueueFamilyIndex();
    VK_CHECK(vkCreateCommandPool(g_Resources.device, &poolInfo, nullptr, &g_Resources.commandPool));
}

void CreateCommandBufferVulkan()
{
    VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocInfo.commandPool = g_Resources.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    g_Resources.commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    VK_CHECK(vkAllocateCommandBuffers(g_Resources.device, &allocInfo, g_Resources.commandBuffers.data()));

    // keep your oneTimeCommandBuffer if you want
    allocInfo.commandBufferCount = 1;
    VK_CHECK(vkAllocateCommandBuffers(g_Resources.device, &allocInfo, &g_Resources.oneTimeCommandBuffer));
}


void CreateSyncObjects()
{
    g_Resources.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkFenceCreateInfo fenceInfo{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        VK_CHECK(vkCreateFence(g_Resources.device, &fenceInfo, nullptr, &g_Resources.inFlightFences[i]));

    // one per swapchain image
    g_Resources.imagesInFlight.resize(g_Resources.swapChain->Images.size(), VK_NULL_HANDLE);
}

void CreateRenderFinishedPerImage()
{
    const uint32_t imageCount = (uint32_t)g_Resources.swapChain->Images.size();
    g_Resources.renderFinishedPerImage.resize(imageCount);

    for (uint32_t i = 0; i < imageCount; ++i)
        g_Resources.renderFinishedPerImage[i] = std::make_shared<Semaphore>(g_Resources.device);
}


void CreateInflightSemaphores()
{
    g_Resources.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    g_Resources.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        g_Resources.imageAvailableSemaphores[i] = std::make_shared<Semaphore>(g_Resources.device);
        g_Resources.renderFinishedSemaphores[i] = std::make_shared<Semaphore>(g_Resources.device);
    }
}

void InitializeGraphicsResources()
{    
    // Set shader directory
#ifdef IS_PLATFORM_WIN
    auto exeDir = GetExecutableDir();
    g_Resources.appContext.shaderDir = exeDir / "shaders/spirv";
#else
    g_Resources.appContext.shaderDir = "./shaders/spirv";
#endif

    // Create graphics resources
    CreateVertexBuffer(g_Resources);
    CreateRenderPass(g_Resources);
    CreateGraphicsPipeline(g_Resources);
    
}

void OnResizeSwapChainVulkan(void* window, const VkExtent2D* newExtent)
{
    // Wait for device idle before recreating resources
    if (g_Resources.device)
    {
        vkDeviceWaitIdle(g_Resources.device);

        // Destroy old framebuffers
        for (auto fb : g_Resources.appContext.framebuffers) {
            if (fb != VK_NULL_HANDLE) {
                vkDestroyFramebuffer(g_Resources.device, fb, nullptr);
            }
        }
        g_Resources.appContext.framebuffers.clear();

    }
    // Recreate swapchain
    g_Resources.extent = *newExtent;
    g_Resources.window = window;
    g_Resources.swapChain = SwapChain::Create(window, vkewGetPhysicalDevice(), vkewGetDevice(), g_Resources.surface,
        g_Resources.extent, VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        true, false, g_Resources.swapChain);

    // Recreate synchronization objects
    CreateInflightSemaphores();
	CreateRenderFinishedPerImage();
    CreateSyncObjects();    
}

void CreateSwapChainVulkan(void* platformWindow, void* platformInstance)
{
    g_Resources.surface = Surface::Create(vkewGetInstance(), platformInstance, platformWindow);
    vkewCreateDevice(g_Resources.surface->Value, 0);
    g_Resources.surface->GetPhysicalDeviceSurfaceFormats(vkewGetPhysicalDevice());
    g_Resources.device = vkewGetDevice();
    vkGetDeviceQueue(g_Resources.device, vkewGetGraphicsQueueFamilyIndex(), 0, &g_Resources.graphicsQueue);
    CreateCommandPoolVulkan();
    CreateCommandBufferVulkan();
    g_Resources.appContext.physicalDevice = vkewGetPhysicalDevice();
    g_Resources.appContext.device = g_Resources.device;
    g_Resources.window = platformWindow;

    
}

void RenderFrameVulkan(VkCommandBuffer cmd)
{
    VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

    VkRenderPassBeginInfo rp{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
    rp.renderPass = g_Resources.appContext.renderPass;
    rp.framebuffer = g_Resources.appContext.framebuffers[g_Resources.imageIndex];
    rp.renderArea.offset = { 0,0 };
    rp.renderArea.extent = g_Resources.swapChain->Extent;

    VkClearValue clearColor = { {{0.2f, 0.3f, 0.4f, 1.0f}} };
    rp.clearValueCount = 1;
    rp.pClearValues = &clearColor;

    vkCmdBeginRenderPass(cmd, &rp, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, g_Resources.appContext.graphicsPipeline);

    VkBuffer vbs[] = { g_Resources.appContext.vertexBuffer };
    VkDeviceSize offs[] = { 0 };
    vkCmdBindVertexBuffers(cmd, 0, 1, vbs, offs);

    vkCmdDraw(cmd, 3, 1, 0, 0);
    vkCmdEndRenderPass(cmd);

    VK_CHECK(vkEndCommandBuffer(cmd));
}


void PresentFrameVulkan(VkSemaphore renderFinishedSemaphore)
{
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &g_Resources.swapChain->Value;
    presentInfo.pImageIndices = &g_Resources.imageIndex;

    VkResult result = vkQueuePresentKHR(g_Resources.graphicsQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        // Handle swapchain recreation if needed
    }
}
void DrawFrameVulkan()
{
    if (g_Resources.dirtySwapChain)
    {
        OnResizeSwapChainVulkan(g_Resources.window, &g_Resources.extent);
        InitializeGraphicsResources();
        g_Resources.dirtySwapChain = false;
    }
    const uint32_t currentFrame = g_Resources.frameIndex % MAX_FRAMES_IN_FLIGHT;

    // Wait for GPU to finish the frame resources we are about to reuse
    VK_CHECK(vkWaitForFences(g_Resources.device, 1, &g_Resources.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX));

    // Acquire image
    uint32_t imageIndex = 0;
    VkSemaphore imageAvailable = g_Resources.imageAvailableSemaphores[currentFrame]->value;

    VK_CHECK(vkAcquireNextImageKHR(
        g_Resources.device,
        g_Resources.swapChain->Value,
        UINT64_MAX,
        imageAvailable,
        VK_NULL_HANDLE,
        &imageIndex));

    g_Resources.imageIndex = imageIndex;

    // If this swapchain image is already being used by an older frame, wait for that frame
    if (g_Resources.imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        VK_CHECK(vkWaitForFences(g_Resources.device, 1, &g_Resources.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX));
    }
    g_Resources.imagesInFlight[imageIndex] = g_Resources.inFlightFences[currentFrame];

    // Now we will submit again with this fence
    VK_CHECK(vkResetFences(g_Resources.device, 1, &g_Resources.inFlightFences[currentFrame]));

    // Record command buffer for this frame
    VkCommandBuffer cmd = g_Resources.commandBuffers[currentFrame];
    VK_CHECK(vkResetCommandBuffer(cmd, 0));

    RenderFrameVulkan(cmd); // change signature to take cmd

    // Submit
    VkSemaphore renderFinished = g_Resources.renderFinishedPerImage[imageIndex]->value;

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailable;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinished;

    VK_CHECK(vkQueueSubmit(g_Resources.graphicsQueue, 1, &submitInfo, g_Resources.inFlightFences[currentFrame]));

    PresentFrameVulkan(renderFinished); // pass semaphore in, or keep currentFrame
    g_Resources.frameIndex++;
}


void LogBreakOnError(void)
{
#ifdef IS_PLATFORM_WIN
    OutputDebugStringA("");
#endif
}

void LogMessageVulkan(VKEWMessageLevel level, const char* pszFormat, ...)
{
    va_list args;
    va_start(args, pszFormat);
    vprintf(pszFormat, args);
    char buffer[8192];
    vsnprintf(buffer, sizeof(buffer), pszFormat, args);
#ifdef IS_PLATFORM_WIN
    lstrcatA(buffer, "\n");
    OutputDebugStringA(buffer);
#endif
    va_end(args);
}

void InitializeVulkan()
{
    VKEWSettings settings = {};
    settings.stSize = sizeof(VKEWSettings);
    settings.pApplicationName = "vkewTest Triangle";
    settings.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    settings.pEngineName = "vkewTest";
    settings.pfnLog = LogMessageVulkan;
    settings.pfnOnError = LogBreakOnError;
    settings.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    settings.apiVersion = VK_MAKE_VERSION(1, 2, 0);
    settings.enableValidation = useValidationLayer;
    settings.enableRaytracing = VK_FALSE;
    settings.enableDynamicRendering = VK_FALSE; // Using traditional render pass

    

    if (vkewInit(&settings) != VK_SUCCESS)
    {
        std::cerr << "Failed to initialize Vulkan" << std::endl;
        exit(-1);
    }
}

void CleanupVulkan()
{
    g_Resources.Cleanup();
    vkewDestroy();
}

#ifdef IS_PLATFORM_WIN
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        VkExtent2D newExtent = {
                static_cast<uint32_t>(rect.right - rect.left),
                static_cast<uint32_t>(rect.bottom - rect.top)
        };
        g_Resources.extent = newExtent;
        g_Resources.dirtySwapChain = true;
    }
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateWin32Window(HINSTANCE hInstance)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "VkewTestWindowClass";
    RegisterClass(&wc);

    return CreateWindowEx(0, "VkewTestWindowClass", "Vkew Triangle Test", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);
}

void RunMessageLoop()
{
    MSG msg = {};
    while (true)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) return;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        DrawFrameVulkan();
    }
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    const char* argv[] = { "app", lpCmdLine };
    int argc = 2;
    ParseCommandLineArgs(argc, argv);
    g_Resources.extent.width = 800;
    g_Resources.extent.height = 600;
    HWND hWindow = CreateWin32Window(hInstance);
    if (!hWindow) return -1;

    InitializeVulkan();
    // Set physical device reference
	CreateSwapChainVulkan(hWindow, hInstance);

    ShowWindow(hWindow, SW_SHOW);

    RunMessageLoop();

    
    CleanupVulkan();

    return 0;
}
#else
int main(int argc, char** argv)
{
    ParseCommandLineArgs(argc, const_cast<const char**>(argv));
    std::cerr << "Non-Windows platforms not yet implemented" << std::endl;
    return -1;
}
#endif