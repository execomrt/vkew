#pragma once
#include <memory>
#include <string>
#include <vector>
#include <optional>
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

class Image 
{
public:
    ~Image() ;
    VkResult Create(const VkImageCreateInfo& create_info);
    void GetImageMemoryRequirements(VkMemoryRequirements& memRequirements) const;
    void GetImageSubresourceLayout(const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
	VkImage Value = VK_NULL_HANDLE;

};


using ImageRef = std::shared_ptr<Image>;

class ImageView
{
public:
    ~ImageView();
    VkResult Create(const VkImageViewCreateInfo& create_info);

    VkImageView Value = VK_NULL_HANDLE;
};

using ImageViewRef = std::shared_ptr<ImageView>;


// Cross-platform display mode wrapper
struct DisplayMode
{
    uint32_t width{};
    uint32_t height{};
    uint32_t refreshRate{};  // in millihertz (e.g., 60000 = 60Hz)

#ifdef VK_KHR_display
    VkDisplayKHR display{ VK_NULL_HANDLE };
    VkDisplayModeKHR mode{ VK_NULL_HANDLE };
    VkDisplayModePropertiesKHR properties{};
#endif  
};

struct DisplayInfo
{
    std::string name;
    uint32_t width{};
    uint32_t height{};
    std::vector<DisplayMode> modes;

#ifdef VK_KHR_display
    VkDisplayKHR display{ VK_NULL_HANDLE };
    VkDisplayPropertiesKHR properties{};
#endif
};


class Surface
{
public:
    VkSurfaceKHR Value{ VK_NULL_HANDLE };
    std::vector<VkSurfaceFormatKHR> formats;

    Surface() = default;

    ~Surface()
    {
        Destroy();
    }

    // Delete copy, allow move
    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;
    Surface(Surface&& other) noexcept : Value(other.Value), formats(std::move(other.formats))
    {
        other.Value = VK_NULL_HANDLE;
    }
    Surface& operator=(Surface&& other) noexcept
    {
        if (this != &other)
        {
            Destroy();
            Value = other.Value;
            formats = std::move(other.formats);
            other.Value = VK_NULL_HANDLE;
        }
        return *this;
    }

    void Destroy()
    {
        if (Value != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(vkewGetInstance(), Value, vkewGetAllocationCallbacks());
            Value = VK_NULL_HANDLE;
        }
    }

    // Windowed surface creation
    static std::shared_ptr<Surface> Create(VkInstance instance, void* platformHandle, void* platformWindow);

#ifdef VK_KHR_display
    // Fullscreen surface creation using VK_KHR_display
    static std::shared_ptr<Surface> CreateFullscreen(
        VkInstance instance,
        VkPhysicalDevice physicalDevice,
        const DisplayMode& displayMode);

    // Query all available displays
    static std::vector<DisplayInfo> EnumerateDisplays(VkPhysicalDevice physicalDevice);

    // Query display modes for a specific display
    static std::vector<DisplayMode> EnumerateDisplayModes(
        VkPhysicalDevice physicalDevice,
        VkDisplayKHR display);

    // Search for a display mode matching criteria
    static std::optional<DisplayMode> FindDisplayMode(
        VkPhysicalDevice physicalDevice,
        uint32_t width,
        uint32_t height,
        uint32_t minRefreshRate = 0);

    // Get the best (highest resolution, highest refresh rate) display mode
    static std::optional<DisplayMode> GetBestDisplayMode(VkPhysicalDevice physicalDevice);

    // Get native display mode (first mode, typically native resolution)
    static std::optional<DisplayMode> GetNativeDisplayMode(VkPhysicalDevice physicalDevice);
#endif

    // Query surface formats for a physical device
    void GetPhysicalDeviceSurfaceFormats(VkPhysicalDevice physicalDevice);

private:
#ifdef VK_KHR_display
    static std::optional<uint32_t> FindCompatiblePlane(
        VkPhysicalDevice physicalDevice,
        VkDisplayKHR display);
#endif
};


using SurfaceRef = std::shared_ptr<Surface>;

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
    std::vector<ImageRef> Images;
    std::vector<ImageViewRef> ImageViews;
    VkExtent2D Extent;
    int GetImageCount() const { return static_cast<int>(Images.size()); }

    VkResult AcquireFullScreenExclusiveMode();
    VkResult ReleaseFullScreenExclusiveMode();
    VkResult GetSwapchainImages(uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
    VkResult AcquireNextImage(uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
    VkResult WaitForPresent(uint64_t presentId, uint64_t timeout);

    static uint32_t GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkSurfaceFormatKHR GetSwapChainFormat(const VkSurfaceFormatKHR* surface_formats, int count, VkFormat colorFormat,
        VkColorSpaceKHR colorSpace);
    static VkExtent2D GetSwapChainExtent(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkImageUsageFlags GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(const VkSurfaceCapabilitiesKHR* surface_capabilities);
    static VkPresentModeKHR GetSwapChainPresentMode(const VkPresentModeKHR* present_modes, int count, int vsync);


};
using SwapChainRef = std::shared_ptr<SwapChain>;