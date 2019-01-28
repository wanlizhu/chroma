#ifndef CHROMA_VULKAN_DEVICE_H
#define CHROMA_VULKAN_DEVICE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include "vulkan_buffer.h"
#include "vulkan_debug_marker.h"

namespace render { namespace device { namespace vulkan {

struct Instance;

struct Device {
    vulkan::Instance* vk_instance = nullptr;
    VkPhysicalDevice physical_device = {};
    VkDevice device = {};
    vulkan::DebugMarker debug_marker;
    bool initialized = false;

    VkPhysicalDeviceProperties props = {};
    VkPhysicalDeviceMemoryProperties memprops = {};
    VkPhysicalDeviceFeatures features = {};
    std::vector<std::string> extensions;
    VkCommandPool command_pool = VK_NULL_HANDLE;
    std::vector<VkQueueFamilyProperties> queue_families;
    struct {
        int graphics = 0;
        int compute = 0;
        int transfer = 0;
    } queue_family_index;

public:
    bool init(vulkan::Instance* inst, 
              VkPhysicalDevice phydev, 
              const std::vector<std::string>& extensions);
    void destroy();
    operator bool() const;
    bool swapchain_enabled() const;
    bool debug_enabled() const;
    bool find_extension(const std::string& ext) const;
    int memory_type_index(uint32_t type_bits, VkMemoryPropertyFlags props) const;

    VkCommandPool create_command_pool(int family_index, VkCommandPoolCreateFlags create_flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    VkCommandBuffer create_command_buffer(VkCommandBufferLevel level, bool begin = false);
    void flush_command_buffer(VkCommandBuffer command_buffer, VkQueue queue, bool free = true);

private:
    void init_queue_family_index();
    bool create_logical_device(const std::vector<std::string>& extensions);
};

}}} // namespace render -> device -> vulkan

#endif