#ifndef CHROMA_VULKAN_FRAMEBUFFER_ATTACHMENT_H
#define CHROMA_VULKAN_FRAMEBUFFER_ATTACHMENT_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>

namespace render { namespace device {  namespace vulkan {

struct FramebufferAttachment {
    VkImage image;
    VkImageView view;
    VkFormat format;
    VkImageSubresourceRange range;
    VkDeviceMemory memory;
    VkAttachmentDescription description;

public:
    bool has_depth() const;
    bool has_stencil() const;
};

}}} // namespace render -> device -> vulkan

#endif