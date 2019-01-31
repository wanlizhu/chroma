#ifndef CHROMA_SYSTEM_NONCOPYABLE_H
#define CHROMA_SYSTEM_NONCOPYABLE_H

namespace sys {

class NonCopyable {
    constexpr NonCopyable(const NonCopyable&) = delete;
    constexpr NonCopyable& operator =(const NonCopyable&) = delete;

protected:
    constexpr NonCopyable() noexcept = default;
    constexpr NonCopyable(NonCopyable&&) noexcept = default;
    constexpr NonCopyable& operator=(NonCopyable&&) noexcept = default;
};

class NonMovable {
    constexpr NonMovable(const NonMovable&) = delete;
    constexpr NonMovable& operator =(const NonMovable&) = delete;
    constexpr NonMovable(NonMovable&&) = delete;
    constexpr NonMovable& operator=(NonMovable&&) = delete;

protected:
    constexpr NonMovable() noexcept = default;
};

}

#endif