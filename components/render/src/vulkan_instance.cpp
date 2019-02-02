#include "vulkan_instance.h"
#include <iostream>
#include <system/c_str.h>
#include <iostream>
#include <sstream>
#include "vulkan_proc.h"
#include "vulkan_adequacy.h"

namespace render { namespace vk {

std::vector<std::string> Instance::BASIC_EXTENSIONS = {
    VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};

std::vector<std::string> Instance::BASIC_LAYERS = {
    "VK_LAYER_LUNARG_standard_validation"
};

Instance::Instance(const std::vector<std::string>& extensions,
                   const std::vector<std::string>& layers) 
    : Properties<VkInstance>(extensions)
    , ValidationLayer(VK_NULL_HANDLE, layers) {
    if (!Adequacy<VkInstance>().require_extensions(extensions)
                               .require_layers(layers)
                               .is_satiable()) {
        throw std::runtime_error("Required extensions or validation layers are not adequate.");
        return;
    }

    sys::StringArrayDeleter deleter;

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "VulkanApplication";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "VulkanEngine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = (uint32_t)extensions.size();
    inst_info.ppEnabledExtensionNames = sys::cstr_array(extensions, &deleter);
    inst_info.enabledLayerCount = (uint32_t)layers.size();
    inst_info.ppEnabledLayerNames = sys::cstr_array(layers, &deleter);

    VkResult res = vkCreateInstance(&inst_info, nullptr, &m_instance);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan Instance!!!");
    }

    if (layers.size() > 0) {
        ValidationLayer::reset(m_instance, layers);
    }
}

Instance::Instance(VkInstance inst) 
    : Properties<VkInstance>({})
    , ValidationLayer(inst, {})
{}

Instance::~Instance() {
    destroy();
}

bool Instance::operator==(const Instance& other) const {
    return m_instance == other.m_instance;
}
       
bool Instance::operator<(const Instance& other) const {
    return m_instance < other.m_instance;
}

Instance::operator bool() const {
    return is_valid();
}

bool Instance::is_valid() const {
    return m_instance != nullptr;
}

void Instance::destroy() {
    ValidationLayer::destroy();
    if (m_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
}

Instance::operator VkInstance() const {
    return m_instance;
}

std::vector<VkPhysicalDevice> Instance::physical_devices() const {
    std::vector<VkPhysicalDevice> devices;

    uint32_t count = 0;
    vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
    if (count <= 0) return devices;

    devices.resize(count);
    VkResult err = vkEnumeratePhysicalDevices(m_instance, &count, devices.data());
    if (err != VK_SUCCESS) {
        return devices;
    }

    return devices;
}

}} // namespace render -> vk