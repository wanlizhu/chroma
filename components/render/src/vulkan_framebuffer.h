#ifndef CHROMA_VULKAN_FRAMEBUFFER_H
#define CHROMA_VULKAN_FRAMEBUFFER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include "vulkan_framebuffer_attachment.h"

namespace render { namespace device { namespace vulkan {

struct Device;

struct Framebuffer {
    vulkan::Device* device = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
    VkFramebuffer framebuffer;
    VkRenderPass renderpass;
    VkSampler sampler;
    std::vector<vulkan::FramebufferAttachment> attachments;

public:
    bool init(vulkan::Device* dev);
    void destroy();
    uint32_t add_attachment(int width, int height, int layers, VkFormat format, VkImageUsageFlags usage);
    void create_sampler(VkFilter mag_filter, VkFilter min_filter, VkSamplerAddressMode adress_mode);
    void create_renderpass();
};

}}} // namespace render -> device -> vulkan

#endif