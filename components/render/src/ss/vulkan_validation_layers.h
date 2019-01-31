#ifndef CHROMA_RENDER_VALIDATION_LAYERS_H
#define CHROMA_RENDER_VALIDATION_LAYERS_H

#include <vulkan/vulkan.h>
#include "vulkan_types.h"

namespace render { namespace vk {

struct ValidationLayers {
    static StrList default_layers;
    static VkDebugReportFlagsEXT default_flags;
    VkInstance instance = VK_NULL_HANDLE;
    std::vector<VkLayerProperties> supported;
    std::map<Str, INDEX> activated;
    VkDebugReportCallbackEXT callback_handle = VK_NULL_HANDLE;

    static ValidationLayers create(StrList const& required_layers, bool abort_on_fail = true);
    bool set_callback(VkInstance inst, VkDebugReportFlagsEXT flags, PFN_vkDebugReportCallbackEXT callback = nullptr);
    void destroy();
    INDEX find(Str const& name) const;
    StrList activated_names() const;

    static VkBool32 VKAPI_CALL default_callback(VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT object_type,
        uint64_t src_object,
        size_t location,
        int32_t code,
        const char* layer_prefix,
        const char* msg,
        void* user_data);
private:
    PFN_vkCreateDebugReportCallbackEXT vk_create_callback = VK_NULL_HANDLE;
    PFN_vkDestroyDebugReportCallbackEXT vk_destroy_callback = VK_NULL_HANDLE;
};

}} // namespace render -> vk

#endif