#include "vulkan_instance.h"
#include <iostream>
#include <system/c_str.h>
#include <iostream>
#include <sstream>
#include "vulkan_proc.h"

namespace render { namespace vk {

namespace details {

VkBool32 VKAPI_CALL default_callback(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT object_type,
    uint64_t src_object,
    size_t location,
    int32_t code,
    const char* layer_prefix,
    const char* msg,
    void* user_data) {
    Instance* ins = reinterpret_cast<Instance*>(user_data);
    if (ins != nullptr && ins->get_callback()) {
        Instance::CallbackInfo info;
        info.object_type = object_type;
        info.src_object = src_object;
        info.location = location;
        info.code = code;
        info.layer = layer_prefix;
        info.message = msg;
        return ins->get_callback()(info);
    }

    std::string prefix("");
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        prefix += "ERROR:";
    }
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        prefix += "WARNING:";
    }
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        prefix += "PERFORMANCE:";
    }
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        prefix += "INFO:";
    }
    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        prefix += "DEBUG:";
    }

    std::stringstream message_stream;
    message_stream << prefix << " [" << layer_prefix << "] Code " << code << " : " << msg;
    std::cerr << message_stream.str() << std::endl;

    //We return VK_FALSE as we DON'T want Vulkan to abort
    return VK_FALSE;
} 

} // namespace details

Instance::CallbackFunction Instance::NULL_CALLBACK;

std::vector<std::string> Instance::EXTENSIONS = {
    VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};

std::vector<std::string> Instance::LAYERS = {
    "VK_LAYER_LUNARG_standard_validation"
};

std::shared_ptr<Instance>
Instance::create(const std::string& name,
                 const std::vector<std::string>& required_extensions,
                 const std::vector<std::string>& required_layers,
                 bool abort_on_fail) {
    std::shared_ptr<Instance> self = std::make_shared<Instance>();
    sys::StringArrayDeleter deleter;

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = name.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = name.c_str();
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = (uint32_t)required_extensions.size();
    inst_info.ppEnabledExtensionNames = sys::cstr_array(required_extensions, &deleter);
    inst_info.enabledLayerCount = (uint32_t)required_layers.size();
    inst_info.ppEnabledLayerNames = sys::cstr_array(required_layers, &deleter);

    VkResult res = vkCreateInstance(&inst_info, nullptr, &self->m_instance);
    if ((res != VK_SUCCESS) && abort_on_fail) {
        throw std::runtime_error("Failed to create Vulkan Instance!!!");
    }

    return self;
}

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

bool Instance::set_callback(VkDebugReportFlagsEXT flags, CallbackFunction callback) {
    m_callback_function = callback;
    VkDebugReportCallbackCreateInfoEXT create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    create_info.pfnCallback = details::default_callback;
    create_info.pUserData = (void*)this;
    create_info.flags = flags;

    return vkCreateDebugReportCallbackEXT(m_instance, &create_info, nullptr, &m_callback_handle) == VK_SUCCESS;
}

Instance::CallbackFunction& Instance::get_callback() {
    return m_callback_function;
}

void Instance::destroy() {
    if (m_callback_handle != VK_NULL_HANDLE) {
        vkDestroyDebugReportCallbackEXT(m_instance, m_callback_handle, nullptr);
        m_callback_handle = VK_NULL_HANDLE;
    }

    if (m_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
}


std::vector<VkPhysicalDevice> Instance::physical_devices(bool print_info) {
    std::vector<VkPhysicalDevice> devices;

    uint32_t count = 0;
    vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
    if (count <= 0) return devices;

    devices.resize(count);
    VkResult err = vkEnumeratePhysicalDevices(m_instance, &count, devices.data());
    if (err != VK_SUCCESS) {
        return devices;
    }

    if (print_info) {
        for (uint32_t i = 0; i < count; i++) {
            VkPhysicalDeviceProperties prop;
            vkGetPhysicalDeviceProperties(devices[i], &prop);

            std::cout << "Device [" << i << "] : " << prop.deviceName << std::endl;
            std::cout << "    Type : " << prop.deviceType << std::endl;
            std::cout << "    API : "
                      << (prop.apiVersion >> 22)
                      << "." << ((prop.apiVersion >> 12) & 0x3ff)
                      << "." << (prop.apiVersion & 0xfff)
                      << std::endl;
        }
    }

    return devices;
}

}} // namespace render -> vk