#ifndef CHROMA_VULKAN_PROPERTIES_H
#define CHROMA_VULKAN_PROPERTIES_H

#include "vulkan_types.h"

namespace render { namespace vk {

template<typename T>
class Properties {
public:
};

template<>
class Properties<VkInstance> {
public:
    std::vector<std::string> supported_extensions() const;
    std::vector<std::string> supported_layers() const;
    uint32_t api_version() const;
};

template<>
class Properties<VkPhysicalDevice> {
public:
    Properties(VkPhysicalDevice phydev);

    VkPhysicalDeviceProperties properties() const;
    VkPhysicalDeviceFeatures features() const;
    VkPhysicalDeviceMemoryProperties memory_properties() const;
    std::vector<std::string> supported_extensions() const; 
    std::vector<VkQueueFamilyProperties> supported_queue_families() const;

private:
    VkPhysicalDevice m_physical_device;
};

}} // namespace render -> vk

#endif