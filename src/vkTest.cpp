/*
 ** The Vulkan Extension Wrangler Library
 ** Copyright (C) 2021, 2025 Stephane Denis
 ** All rights reserved.
 ** V 0.9.8
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
 * 
  */
#ifdef _WIN32
#define IS_PLATFORM_WIN
#endif
#ifdef IS_PLATFORM_WIN
#include <windows.h>
#endif
#include "vkew.h"
#include <iostream>
#include <cstring>
#include <vector>
#define VK_ASSERT(f) { \
    if (!(f)) { \
        std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
}
static VkBool32 useValidationLayer = VK_TRUE;
#include "vkTest.h"
static VulkanResources g_Resources;
void ParseCommandLineArgs(int argc, char* argv[])
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
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = g_Resources.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    VK_CHECK(vkAllocateCommandBuffers(g_Resources.device, &allocInfo, &g_Resources.commandBuffer));
    VK_CHECK(vkAllocateCommandBuffers(g_Resources.device, &allocInfo, &g_Resources.oneTimeCommandBuffer));
}
void CreateSyncObjects() {
    g_Resources.inFlightFences.resize(g_Resources.swapChain->Images.size());
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Initially signaled
    for (size_t i = 0; i < g_Resources.swapChain->Images.size(); ++i) {
        VK_CHECK(vkCreateFence(g_Resources.device, &fenceInfo, nullptr, &g_Resources.inFlightFences[i]));
    }
}
void CreateInflightSemaphores()
{
    int swapChainCount = static_cast<int>(g_Resources.swapChain->Images.size());
    g_Resources.imageAvailableSemaphores.resize(swapChainCount);
    g_Resources.renderFinishedSemaphores.resize(swapChainCount);
    for (int i = 0; i < swapChainCount; i++)
    {
        g_Resources.imageAvailableSemaphores[i] = std::make_shared<Semaphore>(g_Resources.device);
        g_Resources.renderFinishedSemaphores[i] = std::make_shared<Semaphore>(g_Resources.device);
    }
}
void OnResizeSwapChainVulkan(void* window, VkExtent2D* newExtent)
{
    g_Resources.swapChain = SwapChain::Create(window, vkewGetPhysicalDevice(), vkewGetDevice(), g_Resources.surface,
                                              *newExtent, VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                                              true, false, g_Resources.swapChain);
    CreateInflightSemaphores();
    CreateSyncObjects();
}
void CreateSwapChainVulkan(void* platformWindow, void* platformInstance)
{
    g_Resources.surface = Surface::Create(vkewGetInstance(), platformInstance, platformWindow);
    vkewCreateDevice(g_Resources.surface->Value, 0);
    g_Resources.surface->GetPhysicalDeviceSurfaceFormats(vkewGetPhysicalDevice());
    g_Resources.device = vkewGetDevice(); // 1st device
    vkGetDeviceQueue(g_Resources.device, vkewGetGraphicsQueueFamilyIndex(), 0, &g_Resources.graphicsQueue);
    CreateCommandPoolVulkan();
    CreateCommandBufferVulkan();
}
void DestroySwapChainVulkan()
{
    g_Resources.swapChain = nullptr;
    g_Resources.surface = nullptr;
    vkDestroyCommandPool(g_Resources.device, g_Resources.commandPool, nullptr);
}
void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = g_Resources.oneTimeCommandBuffer;
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_GENERAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
    {
        //barrier.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        //barrier.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    else
    {
		VK_ASSERT(0);
    }
    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, 
        destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
    VK_CHECK(vkEndCommandBuffer(commandBuffer));
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    VK_CHECK(vkQueueSubmit(g_Resources.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
    VK_CHECK(vkQueueWaitIdle(g_Resources.graphicsQueue));
}
void WriteCommandBufferVulkan()
{
    VkImage swapChainImage = g_Resources.swapChain->Images[g_Resources.imageIndex];
    VkClearColorValue clearColor = { {0.0f, 1.0f, 0.0f, 1.0f} }; // Solid green
    VkImageSubresourceRange range = {};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;
    vkCmdClearColorImage(g_Resources.commandBuffer, swapChainImage, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &range);
}
void RenderFrameVulkan()
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VK_CHECK(vkBeginCommandBuffer(g_Resources.commandBuffer, &beginInfo));
    WriteCommandBufferVulkan();
    VK_CHECK(vkEndCommandBuffer(g_Resources.commandBuffer));
}
void PresentFrameVulkan()
{
    VkSemaphore renderFinishedSemaphore = g_Resources.renderFinishedSemaphores[g_Resources.GetSwapChainIndex()]->value;
    // Present
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &g_Resources.swapChain->Value;
    presentInfo.pImageIndices = &g_Resources.imageIndex;
    VkResult result = vkQueuePresentKHR(g_Resources.graphicsQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        // Handle swapchain recreation
    }
}
void DrawFrameVulkan() {
    VK_ASSERT(g_Resources.swapChain);
    VK_ASSERT(g_Resources.device);
    // Wait for the fence of the current frame
    vkWaitForFences(g_Resources.device, 1, &g_Resources.inFlightFences[g_Resources.GetSwapChainIndex()], VK_TRUE, UINT64_MAX);
    vkResetFences(g_Resources.device, 1, &g_Resources.inFlightFences[g_Resources.GetSwapChainIndex()]);
    // Acquire next image
    uint32_t imageIndex;
    VkSemaphore imageAvailableSemaphore = g_Resources.imageAvailableSemaphores[g_Resources.GetSwapChainIndex()]->value;
    VK_CHECK(vkAcquireNextImageKHR(g_Resources.device, g_Resources.swapChain->Value, UINT64_MAX,
        imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));
    g_Resources.imageIndex = imageIndex;
    TransitionImageLayout(g_Resources.swapChain->Images[imageIndex], g_Resources.swapChain->Format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
    RenderFrameVulkan();
    // Submit command buffer with current frame's semaphores and fence
    VkSemaphore renderFinishedSemaphore = g_Resources.renderFinishedSemaphores[g_Resources.GetSwapChainIndex()]->value;
    VkSubmitInfo submitInfo = {};
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &g_Resources.commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphore;
    VK_CHECK(vkQueueSubmit(g_Resources.graphicsQueue, 1, &submitInfo, g_Resources.inFlightFences[g_Resources.GetSwapChainIndex()]));
	TransitionImageLayout(g_Resources.swapChain->Images[imageIndex], g_Resources.swapChain->Format, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    PresentFrameVulkan();
    vkQueueWaitIdle(g_Resources.graphicsQueue);
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
    settings.pApplicationName = "Application";
    settings.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    settings.pEngineName = "vkewTest";
    settings.pfnLog = LogMessageVulkan;
    settings.pfnOnError = LogBreakOnError;
    settings.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    settings.apiVersion = VK_MAKE_VERSION(1, 3, 0);
    settings.enableValidation = useValidationLayer;
    settings.enableRaytracing = VK_FALSE;
    settings.enableDynamicRendering = VK_TRUE;
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
        if (wParam != SIZE_MINIMIZED && g_Resources.device)
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            VkExtent2D newExtent = {
                static_cast<uint32_t>(rect.right - rect.left), static_cast<uint32_t>(rect.bottom - rect.top)
            };
            OnResizeSwapChainVulkan(hWnd, &newExtent);
        }
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
HWND CreateWin32Window(HINSTANCE hInstance)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TestWindowClass";
    RegisterClass(&wc);
    return CreateWindowEx(0, "TestWindowClass", "Test", WS_OVERLAPPEDWINDOW,
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
    char* argv[] = {"app", lpCmdLine};
    int argc = 2;
    ParseCommandLineArgs(argc, argv);
    HWND hWindow = CreateWin32Window(hInstance);
    if (!hWindow) return -1;
    InitializeVulkan();
    CreateSwapChainVulkan(hWindow, hInstance);
    ShowWindow(hWindow, SW_SHOW);
    RunMessageLoop();
    DestroySwapChainVulkan();
    CleanupVulkan();
    return 0;
}
#else
int main(int arg, char** argv)
{
    return -1;
}
#endif
