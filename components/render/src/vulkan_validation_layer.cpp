#include "vulkan_validation_layer.h"
#include <sstream>

namespace render { namespace vk {

ValidationLayer::CallbackFunction ValidationLayer::NULL_CALLBACK;

namespace details {

VkBool32 VKAPI_CALL default_callback(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT object_type,
    uint64_t src_object,
    size_t location,
    int32_t code,
    const char* layer_prefix,
    const char* msg,
    void* user_data) {
    ValidationLayer* layer = reinterpret_cast<ValidationLayer*>(user_data);
    if (layer != nullptr && layer->get_callback()) {
        ValidationLayer::CallbackInfo info;
        info.object_type = object_type;
        info.src_object = src_object;
        info.location = location;
        info.code = code;
        info.layer = layer_prefix;
        info.message = msg;
        return layer->get_callback()(info);
    }

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

} // namespace details


ValidationLayer::ValidationLayer(VkInstance inst, const std::vector<std::string>& layers)
    : m_instance(inst)
    , m_callback_handle(VK_NULL_HANDLE)
    , m_enabled_layers(layers)
{}

ValidationLayer::~ValidationLayer() {
    destroy();
}

bool ValidationLayer::set_callback(VkDebugReportFlagsEXT flags, CallbackFunction callback) {
    if (m_instance == VK_NULL_HANDLE)
        return false;

    destroy();
    m_callback_function = callback;

    VkDebugReportCallbackCreateInfoEXT create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    create_info.pfnCallback = details::default_callback;
    create_info.pUserData = (void*)this;
    create_info.flags = flags;

    return vkCreateDebugReportCallbackEXT(m_instance, &create_info, nullptr, &m_callback_handle) == VK_SUCCESS;
}

ValidationLayer::CallbackFunction& ValidationLayer::get_callback() {
    return m_callback_function;
}

void ValidationLayer::reset(VkInstance inst, const std::vector<std::string>& layers) {
    m_instance = inst;
    m_enabled_layers = layers;
}

void ValidationLayer::destroy() {
    if (m_callback_handle != VK_NULL_HANDLE) {
        vkDestroyDebugReportCallbackEXT(m_instance, m_callback_handle, nullptr);
        m_callback_handle = VK_NULL_HANDLE;
    }
}

ValidationLayer::operator bool() const {
    return is_valid();
}

bool ValidationLayer::is_valid() const {
    return (m_callback_handle != VK_NULL_HANDLE) && (m_enabled_layers.size() > 0);
}

const std::vector<std::string>& ValidationLayer::enabled_layers() const {
    return m_enabled_layers;
}


}} // namespace render -> vk