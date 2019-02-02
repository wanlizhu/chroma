#ifndef CHROMA_RENDER_VULKAN_TYPES_H
#define CHROMA_RENDER_VULKAN_TYPES_H

#include <string>
#include <vector>
#include <map>
#include <numeric/vec4.h>

#include <vulkan/vulkan.h>
#if defined(_WIN32)
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(MACOS)
#include <vulkan/vulkan_macos.h>
#elif defined(IOS)
#include <vulkan/vulkan_ios.h>
#elif defined(ANDROID)
#include <vulkan/vulkan_android.h>
#endif

namespace render { namespace vk {

typedef uint32_t INDEX;
typedef uint64_t OBJECT;
typedef uint64_t TAGNAME;

static const INDEX NULL_INDEX = (INDEX)-1;

struct QUEUE_INDEX {
    INDEX family = NULL_INDEX;
    INDEX queue = NULL_INDEX;

    operator bool() const;
    bool is_valid() const;
    bool operator<(const QUEUE_INDEX& idx) const;
    bool operator==(const QUEUE_INDEX& idx) const;
};

}} // namespace render -> vk

namespace std {

template<>
struct hash<render::vk::QUEUE_INDEX> {
    typedef render::vk::QUEUE_INDEX argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const {
        result_type const h1(std::hash<uint32_t>()(s.family));
        result_type const h2(std::hash<uint32_t>()(s.queue));
        return h1 ^ (h2 << 1); 
    }
};

} // namespace std

#endif