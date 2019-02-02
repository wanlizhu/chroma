#include "vulkan_types.h"

namespace render { namespace vk {

QUEUE_INDEX::operator bool() const {
    return is_valid();
}

bool QUEUE_INDEX::is_valid() const {
    return (family != NULL_INDEX) && (queue != NULL_INDEX);
}

bool QUEUE_INDEX::operator<(const QUEUE_INDEX& idx) const {
    union {
        struct { uint32_t x, y; };
        uint64_t i64;
    } uni[2];

    uni[0].x = family;
    uni[0].y = queue;
    uni[1].x = idx.family;
    uni[1].y = idx.queue;

    return uni[0].i64 < uni[1].i64;
}

bool QUEUE_INDEX::operator==(const QUEUE_INDEX& idx) const {
    return (family == idx.family) && (queue == idx.queue);
}

}} // namespace render -> vk