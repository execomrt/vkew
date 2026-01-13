#pragma once
#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include "vkSwapChain.h"


/// @brief Vertex structure matching the shaders
struct Vertex
{
    float position[3];  // x, y, z
    float color[4];     // r, g, b, a
};

/// @brief Application context for graphics resources
struct VulkanAppContext
{
    VkDevice device = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    // Vertex buffer
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

    // Shaders
    VkShaderModule vertexShader = VK_NULL_HANDLE;
    VkShaderModule fragmentShader = VK_NULL_HANDLE;

    // Pipeline
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;

    // Framebuffers (one per swapchain image)
    std::vector<VkFramebuffer> framebuffers;

    // Shader directory path
    std::filesystem::path shaderDir;

    void Cleanup(VkDevice dev);
};

static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

struct VulkanResources {
    VkDevice device;
    VkQueue graphicsQueue;
    VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
    
    VkCommandBuffer oneTimeCommandBuffer;
    std::shared_ptr<Surface> surface;
    std::shared_ptr<SwapChain> swapChain;
    uint32_t imageIndex = 0;
    uint32_t frameIndex = 0;
    std::vector<SemaphoreRef> imageAvailableSemaphores;
    std::vector<SemaphoreRef> renderFinishedSemaphores;
	std::vector<SemaphoreRef> renderFinishedPerImage;
    std::vector<VkFence> inFlightFences;
    VkExtent2D extent;
	void* window = nullptr; 
	bool dirtySwapChain = false;    
    // Application context for graphics
    VulkanAppContext appContext;
	std::vector<VkFence> imagesInFlight;

    int GetSwapChainIndex() const { return frameIndex % swapChain->Images.size(); }

    void Cleanup()
    {
        for (size_t i = 0; i < inFlightFences.size(); ++i) {
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }
        imageAvailableSemaphores.clear();
        renderFinishedSemaphores.clear();

        appContext.Cleanup(device);
    }
};

// Helper functions
bool LoadShaderSPIRV(const std::filesystem::path& filepath, std::vector<char>& outCode);
VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
void CreateVertexBuffer(VulkanResources& resources);
void CreateRenderPass(VulkanResources& resources);
void CreateGraphicsPipeline(VulkanResources& resources);
void CreateFramebuffers(VulkanResources& resources);