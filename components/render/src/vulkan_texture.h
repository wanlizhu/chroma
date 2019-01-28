#ifndef CHROMA_VULKAN_TEXTURE_H
#define CHROMA_VULKAN_TEXTURE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>

namespace render { namespace device { namespace vulkan {

struct Device;

struct Texture {
    vulkan::Device* device = nullptr;
    VkImage image;
    VkImageView view;
    VkImageLayout layout;
    VkDeviceMemory memory;
    uint32_t width;
    uint32_t height;
    uint32_t layers;
    VkDescriptorImageInfo descriptor;
    VkSampler sampler;

public:
    bool init(vulkan::Device* dev);
    void destroy();
    void update_descriptor();
};

}}} // namespace render -> device -> vulkan

#endif