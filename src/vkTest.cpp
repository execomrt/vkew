/*
 ** The Vulkan Extension Wrangler Library
 ** Copyright (C) 2021, 2025 Stephane Denis
 ** All rights reserved.
 ** V 0.9.1
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
#include "vkew.h"
#include <iostream>
#include <cstring>


static VkBool32 useValidationLayer = VK_FALSE;

struct VulkanResources {
    VkDevice device;
    VkSwapchainKHR swapChain;
    VkQueue graphicsQueue;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    uint32_t imageIndex;
};

VulkanResources vulkan;

void ParseCommandLineArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--validation") == 0) {
            useValidationLayer = VK_TRUE;
        }
    }
}

void CreateCommandPoolVulkan() {
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = vkewGetGraphicsQueueFamilyIndex();

    VK_CHECK(vkCreateCommandPool(vulkan.device, &poolInfo, nullptr, &vulkan.commandPool));
}

void CreateCommandBufferVulkan() {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vulkan.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VK_CHECK(vkAllocateCommandBuffers(vulkan.device, &allocInfo, &vulkan.commandBuffer));
}

void CreateSwapChainVulkan(void* platformWindow, void* platformInstance) {
    if (vkewCreateSurface(-1, platformInstance, platformWindow, VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan surface" << std::endl;
        exit(-1);
    }

    vulkan.device = vkewGetDevice();
    vulkan.swapChain = vkewGetSwapChain();
    vkGetDeviceQueue(vulkan.device, vkewGetGraphicsQueueFamilyIndex(), 0, &vulkan.graphicsQueue);

    CreateCommandPoolVulkan();
    CreateCommandBufferVulkan();
}

void DestroySwapChainVulkan() {
    vkewReleaseSwapChain();
    vkDestroyCommandPool(vulkan.device, vulkan.commandPool, nullptr);
}

void RenderFrameVulkan() {
    VkImage swapChainImage = vkewGetSwapChainImage(vulkan.imageIndex);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VK_CHECK(vkBeginCommandBuffer(vulkan.commandBuffer, &beginInfo));

    VkClearColorValue clearColor = { {0.0f, 1.0f, 0.0f, 1.0f} }; // Solid green
    VkImageSubresourceRange range = {};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;

    vkCmdClearColorImage(vulkan.commandBuffer, swapChainImage, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &range);

    VK_CHECK(vkEndCommandBuffer(vulkan.commandBuffer));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vulkan.commandBuffer;

    VK_CHECK(vkQueueSubmit(vulkan.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
    VK_CHECK(vkQueueWaitIdle(vulkan.graphicsQueue));
}

void DrawFrameVulkan() {
    VK_CHECK(vkAcquireNextImageKHR(vulkan.device, vulkan.swapChain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &vulkan.imageIndex));
    RenderFrameVulkan();

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &vulkan.swapChain;
    presentInfo.pImageIndices = &vulkan.imageIndex;

    VK_CHECK(vkQueuePresentKHR(vulkan.graphicsQueue, &presentInfo));
}

void InitializeVulkan() {
    if (vkewInit("Application", "vkewTest", VK_MAKE_VERSION(1, 3, 0), useValidationLayer, VK_FALSE) != VK_SUCCESS) {
        std::cerr << "Failed to initialize Vulkan" << std::endl;
        exit(-1);
    }
}

void CleanupVulkan() {
    vkewDestroy();
}

#ifdef _WIN32
#define IS_PLATFORM_WIN
#include <windows.h>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED) {
            vkewReleaseSwapChain();
            RECT rect;
            GetClientRect(hWnd, &rect);
            VkExtent2D newExtent = { static_cast<uint32_t>(rect.right - rect.left), static_cast<uint32_t>(rect.bottom - rect.top) };
            vkewCreateSwapChain(hWnd, 1, newExtent, 0, VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
        }
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateWin32Window(HINSTANCE hInstance) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TestWindowClass";
    RegisterClass(&wc);

    return CreateWindowEx(0, "TestWindowClass", "Test", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);
}

void RunMessageLoop() {
    MSG msg = {};
    while (true) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        DrawFrameVulkan();
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
    char* argv[] = { "app", lpCmdLine };
    int argc = 2;
    ParseCommandLineArgs(argc, argv);

    HWND hWindow = CreateWin32Window(hInstance);
    if (!hWindow) return -1;
    ShowWindow(hWindow, SW_SHOW);

    InitializeVulkan();
    CreateSwapChainVulkan(hWindow, hInstance);
    RunMessageLoop();
    DestroySwapChainVulkan();
    CleanupVulkan();

    return 0;
}

#endif
