#include "vulkan_validation_layers.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

namespace render { namespace vk { 

StrList ValidationLayers::default_layers = 
{
#ifdef ANDROID
    // On Android we need to explicitly select all layers
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_swapchain",
    "VK_LAYER_GOOGLE_unique_objects"
#else
    "VK_LAYER_LUNARG_standard_validation"
#endif
};

VkDebugReportFlagsEXT ValidationLayers::default_flags =
    VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

INDEX ValidationLayers::find(Str const& name) const {
    for (size_t i = 0; i < supported.size(); i++) {
        if (supported[i].layerName == name) {
            return (INDEX)i;
        }
    }

    return NULL_INDEX;
}

ValidationLayers ValidationLayers::create(StrList const& required_layers, bool abort_on_fail) {
    ValidationLayers self;
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    self.supported.resize(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, self.supported.data());

    for (size_t i = 0; i < required_layers.size(); i++) {
        INDEX index = self.find(required_layers[i]);
        if ((index == NULL_INDEX) && abort_on_fail) {
            throw std::runtime_error("Failed to load required Validation Layers!!!");
        }
        else {
            self.activated[required_layers[i]] = index;
        }
    }

    return self;
}

bool ValidationLayers::set_callback(VkInstance inst, VkDebugReportFlagsEXT flags, PFN_vkDebugReportCallbackEXT callback) {
    instance = inst;

    vk_create_callback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
                         vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
    vk_destroy_callback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
                          vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
    if (vk_create_callback == nullptr || vk_destroy_callback == nullptr) {
        return false;
    }

    VkDebugReportCallbackCreateInfoEXT create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    create_info.pfnCallback = callback;
    create_info.flags = flags;

    VkResult err = vk_create_callback(instance, &create_info, nullptr, &callback_handle);
    if (err != VK_SUCCESS) {
        return false;
    }

    return true;
}

StrList ValidationLayers::activated_names() const {
    StrList names;

    for (auto iter = activated.begin(); iter != activated.end(); iter++) {
        names.push_back(iter->first);
    }

    return names;
}

VkBool32 VKAPI_CALL ValidationLayers::default_callback(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT object_type,
    uint64_t src_object,
    size_t location,
    int32_t code,
    const char* layer_prefix,
    const char* msg,
    void* user_data) {
    std::string prefix("");

    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        prefix += "ERROR:";
    }

    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        prefix += "WARNING:";
    }

    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        prefix += "PERFORMANCE:";
    }

    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        prefix += "INFO:";
    }

    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        prefix += "DEBUG:";
    }

    std::stringstream message_stream;
    message_stream << prefix << " [" << layer_prefix << "] Code " << code << " : " << msg;
    std::cerr << message_stream.str() << std::endl;

    //We return VK_FALSE as we DON'T want Vulkan to abort
    return VK_FALSE;
}

void ValidationLayers::destroy() {
    if (callback_handle != VK_NULL_HANDLE && vk_destroy_callback != VK_NULL_HANDLE) {
        vk_destroy_callback(instance, callback_handle, nullptr);
        callback_handle = VK_NULL_HANDLE;

        instance = VK_NULL_HANDLE;
        supported.clear();
        activated.clear();
    }
}


}} // namespace render -> vk