#include "vulkan_adequacy.h"
#include "system/container.h"

namespace render { namespace vk {

Adequacy<VkInstance>::Adequacy()
    : m_state(true) 
{}

Adequacy<VkInstance>& Adequacy<VkInstance>::require_extensions(const std::vector<std::string>& extensions) {
    if (!m_state)
        return *this;

    m_state = sys::is_subset_of(extensions, m_properties.supported_extensions());

    return *this;
}

Adequacy<VkInstance>& Adequacy<VkInstance>::require_layers(const std::vector<std::string>& layers) {
    if (!m_state)
        return *this;

    m_state = sys::is_subset_of(layers, m_properties.supported_layers());

    return *this;
}

bool Adequacy<VkInstance>::is_satiable() const {
    return m_state;
}

Adequacy<VkPhysicalDevice>::Adequacy(const VkPhysicalDevice& phydev)
    : m_physical_device(phydev)
    , m_state(true)
{}

Adequacy<VkPhysicalDevice>& Adequacy<VkPhysicalDevice>::require_extensions(const std::vector<std::string>& extensions) {
    if (!m_state)
        return *this;

    Properties<VkPhysicalDevice> properties(m_physical_device);
    m_state = sys::is_subset_of(extensions, properties.supported_extensions());

    return *this;
}

Adequacy<VkPhysicalDevice>& Adequacy<VkPhysicalDevice>::require_queue_family(VkQueueFlags families) {
    if (!m_state)
        return *this;

    

    return *this;
}

Adequacy<VkPhysicalDevice>& Adequacy<VkPhysicalDevice>::support(VkSurfaceKHR surface) {
    if (!m_state)
        return *this;

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, surface, &capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, nullptr);
    formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, formats.data());

    uint32_t mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &mode_count, nullptr);
    present_modes.resize(mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &mode_count, present_modes.data());

    m_state = (!formats.empty() && !present_modes.empty());

    return *this;
}

bool Adequacy<VkPhysicalDevice>::is_satiable() const {
    return m_state;
}

}} // namespace render -> vk