#ifndef CHROMA_RENDER_EXTENSION_PROPERTIES_H
#define CHROMA_RENDER_EXTENSION_PROPERTIES_H

#include <vulkan/vulkan.h>
#include "vulkan_types.h"

namespace render { namespace vk {

struct ExtensionProperties {
    std::vector<VkExtensionProperties> supported;
    std::map<std::string, INDEX> activated;

    void reset();
    std::vector<std::string> activated_names() const;
    INDEX find(std::string const& name) const;
};

struct InstanceExtensions : public ExtensionProperties {
    static StrList default_extensions;

    static InstanceExtensions create(StrList const& required, bool abort_on_fail = true);
    bool is_debug_activated() const;
    bool is_surface_activated() const;
};

struct DeviceExtensions : public ExtensionProperties {
    static StrList default_extensions;

    static DeviceExtensions create(VkPhysicalDevice phydev, StrList const& required, bool abort_on_fail = true);
    bool is_debugmarker_activated() const;
};


}} // namespace render -> vk

#endif