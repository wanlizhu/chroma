#ifndef CHROMA_SYSTEM_OPTIONAL_H
#define CHROMA_SYSTEM_OPTIONAL_H

#include <system/compiler.h>
#include <type_traits>
#include <typeinfo>

namespace sys {

template<typename T>
class optional {
public:
    constexpr optional() noexcept : m_initialized(false) {}
    constexpr optional(const T& v) noexcept : m_initialized(true), m_storage(v) {}
    template<typename U>
    optional(const U& other)
        : m_initialized(true)
        , m_storage(other)
    {}

    constexpr bool operator bool() const noexcept { return m_initialized; }
    constexpr bool has_value() const noexcept { return m_initialized; }
    constexpr T& value() noexcept { return m_storage; }
    constexpr const T& value() const noexcept { return m_storage; }
    template<typename U>
    constexpr T value_or(U&& def) const noexcept { if (m_initialized) return m_storage; else return def; }
    void reset() { m_initialized = false; }
    
private:
    bool m_initialized = false;
    T m_storage;
};

} // namespace sys

#endif