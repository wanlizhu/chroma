#include "vulkan_properties.h"
#include "vulkan_instance.h"

namespace render { namespace vk {

std::vector<std::string> Properties<VkInstance>::supported_extensions() const {
    uint32_t count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &count, nullptr);
    std::vector<VkExtensionProperties> extensions(count);
    vkEnumerateInstanceExtensionProperties(NULL, &count, extensions.data());

    std::vector<std::string> names(extensions.size());
    for (size_t i = 0; i < extensions.size(); i++) {
        names[i] = extensions[i].extensionName;
    }

    return names;
}

std::vector<std::string> Properties<VkInstance>::supported_layers() const {
    uint32_t count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    std::vector<VkLayerProperties> layers(count);
    vkEnumerateInstanceLayerProperties(&count, layers.data());

    std::vector<std::string> names(layers.size());
    for (size_t i = 0; i < layers.size(); i++) {
        names[i] = layers[i].layerName;
    }
             
    return names;
}

uint32_t Properties<VkInstance>::api_version() const {
    uint32_t version = 0;
    vkEnumerateInstanceVersion(&version);
    return version;
}

Properties<VkPhysicalDevice>::Properties(VkPhysicalDevice phydev)
    : m_physical_device(phydev) {

}

VkPhysicalDeviceProperties Properties<VkPhysicalDevice>::properties() const {
    assert(m_physical_device != VK_NULL_HANDLE);
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(m_physical_device, &properties);
    return properties;
}

VkPhysicalDeviceFeatures Properties<VkPhysicalDevice>::features() const {
    assert(m_physical_device != VK_NULL_HANDLE);
    VkPhysicalDeviceFeatures features = {};
    vkGetPhysicalDeviceFeatures(m_physical_device, &features);
    return features;
}

VkPhysicalDeviceMemoryProperties Properties<VkPhysicalDevice>::memory_properties() const {
    assert(m_physical_device != VK_NULL_HANDLE);
    VkPhysicalDeviceMemoryProperties mem_properties = {};
    vkGetPhysicalDeviceMemoryProperties(m_physical_device, &mem_properties);
    return mem_properties;
}

std::vector<std::string> Properties<VkPhysicalDevice>::supported_extensions() const {
    assert(m_physical_device != VK_NULL_HANDLE);

    uint32_t count = 0;
    vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> extensions(count);
    vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &count, &extensions.front());

    std::vector<std::string> names(extensions.size());
    for (size_t i = 0; i < extensions.size(); i++) {
        names[i] = extensions[i].extensionName;
    }

    return names;
}
std::vector<VkQueueFamilyProperties> Properties<VkPhysicalDevice>::supported_queue_families() const {
    assert(m_physical_device != VK_NULL_HANDLE);
    
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> qf_properties(count);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &count, qf_properties.data());

    return qf_properties;
}

}} // namespace render-> vk