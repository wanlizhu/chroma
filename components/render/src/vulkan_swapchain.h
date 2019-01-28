#ifndef CHROMA_VULKAN_SWAPCHAIN_H
#define CHROMA_VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>

namespace render { namespace device { namespace vulkan {

struct Device;

struct SwapchainBuffer {
    VkImage image;
    VkImageView view;
};

struct Swapchain {
    vulkan::Device* device = nullptr;
    VkSurfaceKHR surface;
    VkFormat format;
    VkColorSpaceKHR color_space;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> images;
    std::vector<SwapchainBuffer> buffers;
    uint32_t queue_family_index = UINT32_MAX;

    PFN_vkGetPhysicalDeviceSurfaceSupportKHR pfn_GetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR pfn_GetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR pfn_GetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR pfn_GetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkCreateSwapchainKHR pfn_CreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR pfn_DestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR pfn_GetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR pfn_AcquireNextImageKHR;
    PFN_vkQueuePresentKHR pfn_QueuePresentKHR;

public:
    bool connect(vulkan::Device* dev);
    bool create(uint32_t width, uint32_t height, bool vsync = false);
    void init_surface(void* handle, void* window);
    void destroy();
    
    bool acquire_image(VkSemaphore present_complete, uint32_t* image_index);
    bool present_image(VkQueue queue, uint32_t image_index, VkSemaphore wait = VK_NULL_HANDLE);
};

}}} // namespace render -> device -> vulkan

#endif