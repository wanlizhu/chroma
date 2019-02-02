#ifndef CHROMA_RENDER_VULKAN_TYPES_H
#define CHROMA_RENDER_VULKAN_TYPES_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <map>

namespace render { namespace vk {

typedef uint64_t OBJECT;
typedef uint64_t TAGNAME;

struct INDEX {
    int value = -1;

    INDEX(int v = -1) : value(v) {}
    operator bool() const { return is_valid(); }
    bool is_valid() const { return value != -1; }
    operator int() const { return value; }
    operator uint32_t() const { return value; }
    operator uint64_t() const { return value; }
    bool operator<(const INDEX& index) const { return value < index.value; }
    bool operator==(const INDEX& index) const { return value == index.value; }
};

}} // namespace render -> vk

namespace std {

template<>
struct hash<::render::vk::INDEX> {
    typedef ::render::vk::INDEX argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const {
        return std::hash<int>()(s.value);
    }
};


} // namespace std

#endif