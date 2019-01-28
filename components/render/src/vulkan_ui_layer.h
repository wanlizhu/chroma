#ifndef CHROMA_VULKAN_UI_LAYER_H
#define CHROMA_VULKAN_UI_LAYER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include "vulkan_buffer.h"
#include <numeric/vec2.h>

namespace render { namespace device { namespace vulkan {

struct Device;

struct UILayer {
    vulkan::Device* device = nullptr;
    VkQueue queue;
    VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    uint32_t subpass = 0;

    vulkan::Buffer vertex_buffer;
    vulkan::Buffer index_buffer;
    int32_t vertex_count = 0;
    int32_t index_count = 0;

    VkDescriptorPool descriptor_pool;
    VkDescriptorSetLayout descriptorset_layout;
    VkDescriptorSet descriptorset;
    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;

    VkDeviceMemory font_memory = VK_NULL_HANDLE;
    VkImage font_image = VK_NULL_HANDLE;
    VkImageView font_view = VK_NULL_HANDLE;
    VkSampler sampler;

    struct PushConstBlock {
        numeric::float2 scale;
        numeric::float2 translate;
    } push_const_block;

    bool visible = true;
    bool updated = false;
    float scale = 1.0f;
};

}}} // namespace render -> device -> vulkan

#endif