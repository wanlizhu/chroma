#ifndef CHROMA_RENDER_DEBUG_MARKER_H
#define CHROMA_RENDER_DEBUG_MARKER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include "vulkan_types.h"

namespace render { namespace vk { 


struct DebugMarker {
    VkDevice device = VK_NULL_HANDLE;

    static DebugMarker create(VkDevice dev, bool abort_on_fail = true);
    void destroy();
    bool is_valid() const;
    bool set_object_name(OBJECT object, VkDebugReportObjectTypeEXT type, Str const& name);
    bool set_object_tag(OBJECT object, VkDebugReportObjectTypeEXT type, TAGNAME tag_name, size_t tag_size, const void* tag);
    bool begin_region(VkCommandBuffer cmdbuffer, Str const& marker_name, numeric::float4 color);
    bool insert(VkCommandBuffer cmdbuffer, Str const& marker_name, numeric::float4 color);
    bool end_region(VkCommandBuffer cmdbuffer);

    bool set_commandbuffer_name(VkCommandBuffer cmdbuffer, Str const& name);
    bool set_queue_name(VkQueue queue, Str const& name);
    bool set_image_name(VkImage image, Str const& name);
    bool set_sampler_name(VkSampler sampler, Str const& name);
    bool set_buffer_name(VkBuffer buffer, Str const& name);
    bool set_device_memory_name(VkDeviceMemory device_memory, Str const& name);
    bool set_shader_module_name(VkShaderModule shader_module, Str const& name);
    bool set_pipeline_name(VkPipeline pipeline, Str const& name);
    bool set_pipeline_layout_name(VkPipelineLayout pipeline_layout, Str const& name);
    bool set_render_pass_name(VkRenderPass render_pass, Str const& name);
    bool set_framebuffer_name(VkFramebuffer framebuffer, Str const& name);
    bool set_descriptorset_name(VkDescriptorSet desc, Str const& name);
    bool set_descriptorset_layout_name(VkDescriptorSetLayout desc_layout, Str const& name);
    bool set_semaphore_name(VkSemaphore semaphore, Str const& name);
    bool set_fence_name(VkFence fence, Str const& name);
    bool set_event_name(VkEvent ev, Str const& name);

private:
    PFN_vkDebugMarkerSetObjectTagEXT vk_set_object_tag = VK_NULL_HANDLE;
    PFN_vkDebugMarkerSetObjectNameEXT vk_set_object_name = VK_NULL_HANDLE;
    PFN_vkCmdDebugMarkerBeginEXT vk_cmd_begin = VK_NULL_HANDLE;
    PFN_vkCmdDebugMarkerEndEXT vk_cmd_end = VK_NULL_HANDLE;
    PFN_vkCmdDebugMarkerInsertEXT vk_cmd_insert = VK_NULL_HANDLE;
};

}} // namespace render -> vk

#endif