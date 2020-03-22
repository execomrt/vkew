# Vulkan Extension Wrangler
This library allow to initialize Vulkan and adds some extensions. It doesn't requires extra library. The /vulkan folder might not be up to date.

int ret = vkewInit("MyApplication", "MyEngine", VK_API_VERSION_1_0, IsDebug());
switch (ret)
{
  case VK_SUCCESS:
    return 0;
  default:
    return -1;
}

To access objects:

extern VkDevice vkewGetDevice(void);
extern VkPhysicalDevice vkewGetPhysicalDevice(void);
extern VkInstance vkewGetInstance(void);
