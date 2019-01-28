#ifndef CHROMA_VULKAN_INSTANCE_H
#define CHROMA_VULKAN_INSTANCE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include "vulkan_device.h"

namespace render { namespace device {  namespace vulkan {

struct Instance {
    VkInstance instance = {};
    std::vector<std::string> extensions;
    std::vector<std::string> validations;
    VkAllocationCallbacks* allocator = nullptr;

public:
    static const char* error(int code);

    bool init(const char* name,
              const std::vector<std::string>& extensions,
              const std::vector<std::string>& validations,
              VkAllocationCallbacks* allocator = nullptr);
    void destroy();
    void init_debug(VkDebugReportFlagsEXT flags, VkDebugReportCallbackEXT callback);
    std::vector<vulkan::Device> devices(bool init_dev) const;
};

}}} // namespace render -> device -> vulkan

#endif