#ifndef CHROMA_VULKAN_API_H
#define CHROMA_VULKAN_API_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include <system/optional.h>

namespace render { namespace device { 

struct VulkanDevice {
    VkPhysicalDevice physical_device = {};
    VkDevice device = {};
    
    VkPhysicalDeviceProperties props = {};
    VkPhysicalDeviceMemoryProperties memprops = {};
    VkPhysicalDeviceFeatures features = {};
    std::vector<VkQueueFamilyProperties> queue_families;
    std::vector<std::string> extensions;
    VkCommandPool command_pool = VK_NULL_HANDLE;
};

bool create_instance(VkInstance* instance,
                  const std::string& name,
                  const std::vector<const char*>& exts,
                  const std::vector<const char*>& validations,
                  const VkAllocationCallbacks* allocator = nullptr);

std::vector<VulkanDevice> device_list(VkInstance instance, bool print);



const char* error(int code);

}} // namespace render -> device

#endif