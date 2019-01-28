#ifndef CHROMA_VULKAN_BUFFER_H
#define CHROMA_VULKAN_BUFFER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>

namespace render { namespace device { namespace vulkan {

struct Device;

struct Buffer {
    vulkan::Device* device = nullptr;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDescriptorBufferInfo descriptor;
    VkDeviceSize size = 0;
    VkDeviceSize alignment = 0;
    void* mapped = nullptr;

    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags memprops;

public:
    bool init(VkBufferUsageFlags usage, VkMemoryPropertyFlags props, size_t size, const void* data = nullptr);
    void destroy();
    void* map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void unmap();
    bool bind_memory(VkDeviceSize offset);
    void copy_from(const void* data, VkDeviceSize size);
    void copy_to(void* data, VkDeviceSize size);
    bool flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    bool invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
};

}}} // namespace render -> device -> vulkan

#endif