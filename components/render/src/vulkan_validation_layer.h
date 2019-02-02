#ifndef CHROMA_RENDER_VULKAN_VALIDATION_LAYER_H
#define CHROMA_RENDER_VULKAN_VALIDATION_LAYER_H

#include "vulkan_types.h"

namespace render { namespace vk {

class ValidationLayer {
public:
    struct CallbackInfo {
        VkDebugReportFlagsEXT flags;
        VkDebugReportObjectTypeEXT object_type;
        uint64_t src_object;
        size_t location;
        int32_t code;
        const char* layer;
        const char* message;
    };
    typedef std::function<bool(const CallbackInfo&)> CallbackFunction;
    static CallbackFunction NULL_CALLBACK;
    static const VkDebugReportFlagsEXT CALLBACK_FLAGS = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

public:
    bool set_callback(VkDebugReportFlagsEXT flags = CALLBACK_FLAGS, CallbackFunction callback = NULL_CALLBACK);
    CallbackFunction& get_callback();
    
protected:
    ValidationLayer(VkInstance inst, const std::vector<std::string>& layers);
    virtual ~ValidationLayer();
    
    void reset(VkInstance inst, const std::vector<std::string>& layers);
    void destroy();
    operator bool() const;
    bool is_valid() const;
    const std::vector<std::string>& enabled_layers() const;

private:
    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT m_callback_handle = VK_NULL_HANDLE;
    CallbackFunction m_callback_function;
    std::vector<std::string> m_enabled_layers;
};

}} // namespace render -> vk

#endif