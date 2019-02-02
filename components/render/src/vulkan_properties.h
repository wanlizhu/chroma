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
    Properties(const std::vector<std::string>& extensions);

    std::vector<std::string> supported_extensions() const;
    std::vector<std::string> supported_layers() const;
    uint32_t api_version() const;
    const std::vector<std::string>& enabled_extensions() const;

private:
    std::vector<std::string> m_enabled_extensions;
};

template<>
class Properties<VkPhysicalDevice> {
public:
    struct QueueFamilyIndices {
        INDEX graphics;
        INDEX present;
        INDEX compute;
        INDEX transfer;

        operator bool() const;
        bool is_valid() const;
        bool is_dedicated_compute() const;
        bool is_dedicated_transfer() const;
    };

public:
    Properties(VkPhysicalDevice phydev);

    void reset(VkPhysicalDevice phydev);
    VkPhysicalDeviceProperties properties() const;
    VkPhysicalDeviceFeatures features() const;
    VkPhysicalDeviceMemoryProperties memory_properties() const;
    std::vector<std::string> supported_extensions() const; 
    std::vector<VkQueueFamilyProperties> supported_queue_families() const;
    QueueFamilyIndices queue_family_indices(VkSurfaceKHR surface) const;

private:
    VkPhysicalDevice m_physical_device;
};

}} // namespace render -> vk

#endif