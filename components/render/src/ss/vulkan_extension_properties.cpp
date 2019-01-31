#include "vulkan_extension_properties.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

namespace render { namespace vk { 

StrList InstanceExtensions::default_extensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
};

StrList DeviceExtensions::default_extensions = {
    VK_EXT_DEBUG_MARKER_EXTENSION_NAME
};

DeviceExtensions DeviceExtensions::create(VkPhysicalDevice phydev, StrList const& required, bool abort_on_fail) {
    DeviceExtensions self;
    uint32_t supported_count = 0;
    vkEnumerateDeviceExtensionProperties(phydev, nullptr, &supported_count, nullptr);
    self.supported.resize(supported_count);
    vkEnumerateDeviceExtensionProperties(phydev, nullptr, &supported_count, self.supported.data());

    for (size_t i = 0; i < required.size(); i++) {
        INDEX index = self.find(required[i]);
        if ((index == NULL_INDEX) && abort_on_fail) {
            throw std::runtime_error("Failed to load required Device Extension!!!");
        }
        else {
            self.activated[required[i]] = index;
        }
    }

    return self;
}

InstanceExtensions InstanceExtensions::create(StrList const& required, bool abort_on_fail) {
    InstanceExtensions self;
    uint32_t supported_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &supported_count, nullptr);
    self.supported.resize(supported_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &supported_count, self.supported.data());

    for (size_t i = 0; i < required.size(); i++) {
        INDEX index = self.find(required[i]);
        if ((index == NULL_INDEX) && abort_on_fail) {
            throw std::runtime_error("Failed to load required Instance Extension!!!");
        }
        else {
            self.activated[required[i]] = index;
        }
    }

    return self;
}

StrList ExtensionProperties::activated_names() const {
    StrList names;

    for (auto iter = activated.begin(); iter != activated.end(); iter++) {
        names.push_back(iter->first);
    }

    return names;
}

bool InstanceExtensions::is_debug_activated() const {
    return find(VK_EXT_DEBUG_REPORT_EXTENSION_NAME) != NULL_INDEX;
}

bool InstanceExtensions::is_surface_activated() const {
    return find(VK_KHR_SURFACE_EXTENSION_NAME) != NULL_INDEX
#if defined(WINDOWS)
        && find(VK_KHR_WIN32_SURFACE_EXTENSION_NAME) != NULL_INDEX
#elif defined(MACOS)
        && find(VK_MVK_MACOS_SURFACE_EXTENSION_NAME) != NULL_INDEX
#elif defined(IOS)
        && find(VK_MVK_IOS_SURFACE_EXTENSION_NAME) != NULL_INDEX
#elif defined(ANDROID)
        && find(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME) != NULL_INDEX
#endif
        ;
}

void ExtensionProperties::reset() {
    supported.clear();
    activated.clear();
}

INDEX ExtensionProperties::find(Str const& name) const {
    for (size_t i = 0; i < supported.size(); i++) {
        if (supported[i].extensionName == name) {
            return (INDEX)i;
        }
    }

    return NULL_INDEX;
}

bool DeviceExtensions::is_debugmarker_activated() const {
    return find(VK_EXT_DEBUG_MARKER_EXTENSION_NAME) != NULL_INDEX;
}

}} // namespace render -> vk