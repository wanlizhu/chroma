#ifndef CHROMA_VULKAN_ADEQUACY_H
#define CHROMA_VULKAN_ADEQUACY_H

#include "vulkan_properties.h"

namespace render { namespace vk {

template<typename T>
class Adequacy {
public:
};

template<>
class Adequacy<VkInstance> {
public:
    Adequacy();

    Adequacy& require_extensions(const std::vector<std::string>& extensions);
    Adequacy& require_layers(const std::vector<std::string>& layers);
    bool is_satiable() const;

private:
    Properties<VkInstance> m_properties;
    bool m_state = true;
};

template<>
class Adequacy<VkPhysicalDevice> {
public:
    Adequacy(const VkPhysicalDevice& phydev);

    Adequacy& require_extensions(const std::vector<std::string>& extensions);
    Adequacy& require_queue_family(VkQueueFlags families);
    Adequacy& support(VkSurfaceKHR surface);
    bool is_satiable() const;

private:
    const VkPhysicalDevice& m_physical_device;
    bool m_state = true;
};

}} // namespace render -> vk

#endif