#include "vulkan_properties.h"
#include "vulkan_instance.h"
#include <system/container.h>
#include <assert.h>

namespace render { namespace vk {

Properties<VkInstance>::Properties(const std::vector<std::string>& extensions)
    : m_enabled_extensions(extensions) {
    if (!sys::is_subset_of(extensions, supported_extensions())) {
        throw std::runtime_error("Failed to load required instance extensions.");
        return;
    }
}

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

const std::vector<std::string>& Properties<VkInstance>::enabled_extensions() const {
    return m_enabled_extensions;
}

Properties<VkPhysicalDevice>::Properties(VkPhysicalDevice phydev)
    : m_physical_device(phydev) {

}

void Properties<VkPhysicalDevice>::reset(VkPhysicalDevice phydev) {
    m_physical_device = phydev;
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

Properties<VkPhysicalDevice>::QueueFamilyIndices Properties<VkPhysicalDevice>::queue_family_indices(VkSurfaceKHR surface) const {
    auto predicate = [] (VkQueueFamilyProperties family, VkQueueFlags flags)->bool {
        return (family.queueCount > 0) && (family.queueFlags & flags);
    };
    auto support_surface = [&] (VkQueueFamilyProperties family, INDEX index, VkSurfaceKHR surface)->bool {
        VkBool32 support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, (int)index, surface, &support);
        return (family.queueCount > 0) && support;
    };

    QueueFamilyIndices indices;
    std::vector<VkQueueFamilyProperties> families = supported_queue_families();

    for (size_t i = 0; i < families.size(); i++) {
        if (predicate(families[i], VK_QUEUE_GRAPHICS_BIT)) {
            indices.graphics = i;
            break;
        }
    }

    for (size_t i = 0; i < families.size(); i++) {
        if (support_surface(families[i], i, surface)) {
            indices.present = i;
            break;
        }
    }
    if (!indices.present) {
        indices.present = indices.graphics;
    }

    for (size_t i = 0; i < families.size(); i++) {
        if (predicate(families[i], VK_QUEUE_COMPUTE_BIT)) {
            indices.compute = i;
            break;
        }
    }
    if (!indices.compute) {
        indices.compute = indices.graphics;
    }

    for (size_t i = 0; i < families.size(); i++) {
        if (predicate(families[i], VK_QUEUE_TRANSFER_BIT)) {
            indices.transfer = i;
            break;
        }
    }
    if (!indices.transfer) {
        // both compute and graphics family implicitly support transfer
        indices.transfer = indices.graphics ? indices.graphics : indices.compute;
    }

    return indices;
}

Properties<VkPhysicalDevice>::QueueFamilyIndices::operator bool() const {
    return is_valid();
}

bool Properties<VkPhysicalDevice>::QueueFamilyIndices::is_valid() const {
    return graphics.is_valid() 
        && present.is_valid() 
        && compute.is_valid()
        && transfer.is_valid();
}

bool Properties<VkPhysicalDevice>::QueueFamilyIndices::is_dedicated_compute() const {
    return !(compute == graphics) 
        && !(compute == present)
        && !(compute == transfer);
}

bool Properties<VkPhysicalDevice>::QueueFamilyIndices::is_dedicated_transfer() const {
    return !(transfer == graphics)
        && !(transfer == present)
        && !(transfer == compute);
}

}} // namespace render-> vk