#ifndef CHROMA_RENDER_INSTANCE_H
#define CHROMA_RENDER_INSTANCE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include "vulkan_types.h"
#include <system/noncopyable.h>

namespace render { namespace vk {  

class Instance : public sys::NonCopyable {
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
    static std::vector<std::string> EXTENSIONS;
    static std::vector<std::string> LAYERS;

public:
    static std::shared_ptr<Instance>
    create(const std::string& name, 
           const std::vector<std::string>& required_extensions,
           const std::vector<std::string>& required_layers,
           bool abort_on_fail = true);

    Instance() = default;
    virtual ~Instance();

    bool operator==(const Instance& other) const;
    bool operator<(const Instance& other) const;
    operator bool() const;
    void destroy();
    bool is_valid() const;
    bool set_callback(VkDebugReportFlagsEXT flags = CALLBACK_FLAGS, CallbackFunction callback = NULL_CALLBACK);
    CallbackFunction& get_callback();
    std::vector<VkPhysicalDevice> physical_devices(bool print_info = false);

private:
    VkInstance m_instance = VK_NULL_HANDLE;
    std::vector<std::string> m_extensions;
    std::vector<std::string> m_validation_layers;
    VkDebugReportCallbackEXT m_callback_handle = VK_NULL_HANDLE;
    CallbackFunction m_callback_function;
};

}} // namespace render -> vk

#endif