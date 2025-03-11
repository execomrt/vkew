#pragma once
/// @brief Semaphore
class Semaphore
{
public:
    Semaphore(VkDevice aDevice) {
        VkSemaphoreCreateInfo sem_ci = {};
        sem_ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VK_CHECK(vkCreateSemaphore(aDevice, &sem_ci, nullptr, &value));
        device = aDevice;
    }
    ~Semaphore() {
        if (value) {
            vkDestroySemaphore(device, value, nullptr);
        }
    }
    VkDevice device;
    VkSemaphore value;
};
using SemaphoreRef = std::shared_ptr<Semaphore>;
/// @brief Surface
class Surface
{
public:
    VkSurfaceKHR Value;
    std::vector<VkSurfaceFormatKHR> Formats;
    Surface() : Value(VK_NULL_HANDLE) {}
    ~Surface()
    {
        if (Value != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(vkewGetInstance(), Value, nullptr);
            Value = VK_NULL_HANDLE;
        }
    }
    static std::shared_ptr<Surface> Create(VkInstance instance, void* platformHandle, void* platformWindow);
    void GetPhysicalDeviceSurfaceFormats(VkPhysicalDevice physicalDevice);
};
using SurfaceRef = std::shared_ptr<Surface>;
/// @brief Swap chain
class SwapChain
{
public:
    static std::shared_ptr<SwapChain> Create(
        void* platformWindow,
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        SurfaceRef surface,
        VkExtent2D& desired_extent,
        VkFormat colorFormat,
        VkColorSpaceKHR colorSpace,
        bool vsync,
        bool full_screen_exclusive,
        std::shared_ptr<SwapChain> previous);
    ~SwapChain();
    SwapChain() = default;
    VkSwapchainKHR Value;
    VkFormat Format;
    std::vector<VkImage> Images;
    std::vector<VkImageView> ImageViews;
    VkExtent2D Extent;
    static uint32_t GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkSurfaceFormatKHR GetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count, VkFormat colorFormat,
        VkColorSpaceKHR colorSpace);
    static VkExtent2D GetSwapChainExtent(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkImageUsageFlags GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkPresentModeKHR GetSwapChainPresentMode(const VkPresentModeKHR* present_modes, int count, int vsync);
};
using SwapChainRef = std::shared_ptr<SwapChain>;


struct VulkanResources {
    VkDevice device;
    VkQueue graphicsQueue;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkCommandBuffer oneTimeCommandBuffer;
    std::shared_ptr<Surface> surface;
    std::shared_ptr<SwapChain> swapChain;
    uint32_t imageIndex = 0;
    uint32_t frameIndex = 0;
    std::vector<SemaphoreRef> imageAvailableSemaphores;
    std::vector<SemaphoreRef> renderFinishedSemaphores;
    int GetSwapChainIndex() const { return frameIndex % swapChain->Images.size(); }
    void Cleanup()
    {
       
        for (size_t i = 0; i < inFlightFences.size(); ++i) {

            vkDestroyFence(device, inFlightFences[i], nullptr);
        }
        imageAvailableSemaphores.clear();
        renderFinishedSemaphores.clear();
       
    }
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;
    static const int MAX_FRAMES_IN_FLIGHT = 3;
};