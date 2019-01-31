#include "vulkan_debug_marker.h"

namespace render { namespace vk { 

DebugMarker DebugMarker::create(VkDevice dev, bool abort_on_fail) {
    DebugMarker self;
    self.device = dev;
    self.vk_set_object_tag = reinterpret_cast<PFN_vkDebugMarkerSetObjectTagEXT>(vkGetDeviceProcAddr(self.device, "vkDebugMarkerSetObjectTagEXT"));
    self.vk_set_object_name = reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(vkGetDeviceProcAddr(self.device, "vkDebugMarkerSetObjectNameEXT"));
    self.vk_cmd_begin = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>(vkGetDeviceProcAddr(self.device, "vkCmdDebugMarkerBeginEXT"));
    self.vk_cmd_end = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>(vkGetDeviceProcAddr(self.device, "vkCmdDebugMarkerEndEXT"));
    self.vk_cmd_insert = reinterpret_cast<PFN_vkCmdDebugMarkerInsertEXT>(vkGetDeviceProcAddr(self.device, "vkCmdDebugMarkerInsertEXT"));

    if (abort_on_fail && !self.is_valid()) {
        throw std::runtime_error("Failed to load DebugMarker extension!!!");
    }

    return self;
}

void DebugMarker::destroy() {
    device = VK_NULL_HANDLE;
    vk_set_object_tag = VK_NULL_HANDLE;
    vk_set_object_name = VK_NULL_HANDLE;
    vk_cmd_begin = VK_NULL_HANDLE;
    vk_cmd_end = VK_NULL_HANDLE;
    vk_cmd_insert = VK_NULL_HANDLE;
}

bool DebugMarker::is_valid() const {
    if (vk_set_object_name == nullptr
        || vk_set_object_tag == nullptr
        || vk_cmd_begin == nullptr
        || vk_cmd_end == nullptr
        || vk_cmd_insert == nullptr) {
        return false;
    }

    return true;
}

bool DebugMarker::set_object_name(OBJECT object, VkDebugReportObjectTypeEXT type, Str const& name) {
    if (vk_set_object_name) {
        VkDebugMarkerObjectNameInfoEXT name_info = {};
        name_info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
        name_info.objectType = type;
        name_info.object = object;
        name_info.pObjectName = name.c_str();
        return vk_set_object_name(device, &name_info) == VK_SUCCESS;
    }
    return false;
}

bool DebugMarker::set_object_tag(OBJECT object, VkDebugReportObjectTypeEXT type, TAGNAME tag_name, size_t tag_size, const void* tag) {
    if (vk_set_object_tag) {
        VkDebugMarkerObjectTagInfoEXT tag_info = {};
        tag_info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
        tag_info.objectType = type;
        tag_info.object = object;
        tag_info.tagName = tag_name;
        tag_info.tagSize = tag_size;
        tag_info.pTag = tag;
        return vk_set_object_tag(device, &tag_info) == VK_SUCCESS;
    }
    return false;
}

bool DebugMarker::begin_region(VkCommandBuffer cmdbuffer, Str const& marker_name, numeric::float4 color) {
    if (vk_cmd_begin) {
        VkDebugMarkerMarkerInfoEXT markerInfo = {};
        markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        memcpy(markerInfo.color, &color[0], sizeof(float) * 4);
        markerInfo.pMarkerName = marker_name.c_str();
        vk_cmd_begin(cmdbuffer, &markerInfo);
        return true;
    }
    return false;
}

bool DebugMarker::insert(VkCommandBuffer cmdbuffer, Str const& marker_name, numeric::float4 color) {
    if (vk_cmd_insert) {
        VkDebugMarkerMarkerInfoEXT markerInfo = {};
        markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        memcpy(markerInfo.color, &color[0], sizeof(float) * 4);
        markerInfo.pMarkerName = marker_name.c_str();
        vk_cmd_insert(cmdbuffer, &markerInfo);
        return true;
    }
    return false;
}

bool DebugMarker::end_region(VkCommandBuffer cmdbuffer) {
    if (vk_cmd_end) {
        vk_cmd_end(cmdbuffer);
        return true;
    }
    return false;
}

bool DebugMarker::set_commandbuffer_name(VkCommandBuffer cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT, name);
}

bool DebugMarker::set_queue_name(VkQueue cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT, name);
}

bool DebugMarker::set_image_name(VkImage cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, name);
}

bool DebugMarker::set_sampler_name(VkSampler cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, name);
}

bool DebugMarker::set_buffer_name(VkBuffer cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, name);
}

bool DebugMarker::set_device_memory_name(VkDeviceMemory cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, name);
}

bool DebugMarker::set_shader_module_name(VkShaderModule cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, name);
}

bool DebugMarker::set_pipeline_name(VkPipeline cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, name);
}

bool DebugMarker::set_pipeline_layout_name(VkPipelineLayout cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT, name);
}

bool DebugMarker::set_render_pass_name(VkRenderPass cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, name);
}

bool DebugMarker::set_framebuffer_name(VkFramebuffer cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT, name);
}

bool DebugMarker::set_descriptorset_name(VkDescriptorSet cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, name);
}

bool DebugMarker::set_descriptorset_layout_name(VkDescriptorSetLayout cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, name);
}

bool DebugMarker::set_semaphore_name(VkSemaphore cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT, name);
}

bool DebugMarker::set_fence_name(VkFence cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT, name);
}

bool DebugMarker::set_event_name(VkEvent cmdbuffer, Str const& name) {
    return set_object_name((OBJECT)cmdbuffer, VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT, name);
}


}} // namespace render -> vk