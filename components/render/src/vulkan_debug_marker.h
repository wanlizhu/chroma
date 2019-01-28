#ifndef CHROMA_VULKAN_DEBUG_MARKER_H
#define CHROMA_VULKAN_DEBUG_MARKER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include <numeric/vec4.h>

namespace render { namespace device { namespace vulkan {

struct Device;

struct DebugMarker {
    bool enabled = false;
    vulkan::Device* device = nullptr;

    PFN_vkDebugMarkerSetObjectTagEXT pfn_vkDebugMarkerSetObjectTagEXT = VK_NULL_HANDLE;
    PFN_vkDebugMarkerSetObjectNameEXT pfn_vkDebugMarkerSetObjectNameEXT = VK_NULL_HANDLE;
    PFN_vkCmdDebugMarkerBeginEXT pfn_vkCmdDebugMarkerBeginEXT = VK_NULL_HANDLE;
    PFN_vkCmdDebugMarkerEndEXT pfn_vkCmdDebugMarkerEndEXT = VK_NULL_HANDLE;
    PFN_vkCmdDebugMarkerInsertEXT pfn_vkCmdDebugMarkerInsertEXT = VK_NULL_HANDLE;

public:
    bool init(vulkan::Device* dev);
    void destroy();
    void set_object_name(uint64_t object, VkDebugReportObjectTypeEXT objectType, const char *name);
    void set_object_tag(uint64_t object, VkDebugReportObjectTypeEXT objectType, uint64_t name, size_t tagSize, const void* tag);
    
    void begin_region(VkCommandBuffer cmd_buffer, const char* marker_name, numeric::float4 color);
    void insert(VkCommandBuffer cmd_buffer, const char* marker_name, numeric::float4 color);
    void end_region(VkCommandBuffer cmd_buffer);

    void set_cmdbuffer_name(VkCommandBuffer cmdBuffer, const char * name);
    void set_queue_name(VkQueue queue, const char * name);
    void set_image_name(VkImage image, const char * name);
    void set_sampler_name(VkSampler sampler, const char * name);
    void set_buffer_name(VkBuffer buffer, const char * name);
    void set_memory_name(VkDeviceMemory memory, const char * name);
    void set_shader_name(VkShaderModule shaderModule, const char * name);
    void set_pipeline_name(VkPipeline pipeline, const char * name);
    void set_pipeline_layout_name(VkPipelineLayout pipelineLayout, const char * name);
    void set_renderpass_name(VkRenderPass renderPass, const char * name);
    void set_framebuffer_name(VkFramebuffer framebuffer, const char * name);
    void set_descriptorset_layout_name(VkDescriptorSetLayout descriptorSetLayout, const char * name);
    void set_descriptorset_name(VkDescriptorSet descriptorSet, const char * name);
    void set_semaphore_name(VkSemaphore semaphore, const char * name);
    void set_fence_name(VkFence fence, const char * name);
    void set_event_name(VkEvent _event, const char * name);
};

}}} // namespace render -> device -> vulkan

#endif