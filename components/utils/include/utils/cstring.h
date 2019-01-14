#ifndef CHROMA_UTILS_CSTRING_H
#define CHROMA_UTILS_CSTRING_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <functional>

namespace utils {

struct HashCStrings {
    typedef const char* argument_type;
    typedef size_t result_type;
    
    result_type
    operator()(argument_type cstr) const noexcept;
};

struct EqualCStrings {
    typedef const char* first_argument_type;
    typedef const char* second_argument_type;
    typedef bool result_type;
    
    bool
    operator()(const char* lhs, const char* rhs) const noexcept;
};

struct LessCStrings {
    typedef const char* first_argument_type;
    typedef const char* second_argument_type;
    typedef bool result_type;
    
    result_type 
    operator()(first_argument_type lhs, second_argument_type rhs) const noexcept;
};

template <size_t N>
using StringLiteral = const char[N];

class StaticString {
public:
    using value_type = char;
    using size_type = uint32_t;
    using difference_type = int32_t;
    using const_reference = const value_type&;
    using const_pointer = const value_type*;
    using const_iterator = const value_type*;

    constexpr
    StaticString() noexcept = default;

    template <size_t N>
    StaticString(StringLiteral<N> const& other) noexcept 
        : m_string(other),
          m_length(size_type(N - 1)),
          m_hash(compute_hash(other)) {
    }

    template<size_t N>
    StaticString& 
    operator=(StringLiteral<N> const& other) noexcept {
        m_string = other;
        m_length = size_type(N - 1);
        m_hash = compute_hash(other);
        return *this;
    }

    static constexpr StaticString make(const_pointer literal, size_t length) noexcept;
    static StaticString make(const_pointer literal) noexcept;

    inline const_pointer c_str() const noexcept { return m_string; }
    inline const_pointer data() const noexcept { return m_string; }
    inline size_type size() const noexcept { return m_length; }
    inline size_type length() const noexcept { return m_length; }
    inline bool empty() const noexcept { return size() == 0; }
    inline void clear() noexcept { m_string = nullptr; m_length = 0; }

    inline const_iterator begin() const noexcept { return m_string; }
    inline const_iterator end() const noexcept { return m_string + m_length; }
    inline const_iterator cbegin() const noexcept { return begin(); }
    inline const_iterator cend() const noexcept { return end(); }

    inline const_reference operator[](size_type pos) const noexcept { assert(pos < size()); return begin()[pos]; }
    inline const_reference at(size_type pos) const noexcept { assert(pos < size()); return begin()[pos]; }
    inline const_reference front() const noexcept { assert(size()); return begin()[0]; }
    inline const_reference back() const noexcept { assert(size()); return begin()[size() - 1]; }
    inline size_type hash() const noexcept { return m_hash; }

private:
    template<size_t N>
    static constexpr 
    size_type
    compute_hash(StringLiteral<N> const& s) noexcept {
        size_type hash = 5381;
        for (size_t i = 0; i < N - 1; i++) {
            hash = (hash * 33u) ^ size_type(s[i]);
        }
        return hash;
    }

    int compare(const StaticString& rhs) const noexcept;

    inline friend bool operator==(StaticString const& lhs, StaticString const& rhs) noexcept {
        return (lhs.data() == rhs.data()) || ((lhs.size() == rhs.size()) && !strncmp(lhs.data(), rhs.data(), lhs.size()));
    }
    inline friend bool operator!=(StaticString const& lhs, StaticString const& rhs) noexcept {
        return !(lhs == rhs);
    }
    inline friend bool operator<(StaticString const& lhs, StaticString const& rhs) noexcept {
        return lhs.compare(rhs) < 0;
    }
    inline friend bool operator>(StaticString const& lhs, StaticString const& rhs) noexcept {
        return lhs.compare(rhs) > 0;
    }
    inline friend bool operator>=(StaticString const& lhs, StaticString const& rhs) noexcept {
        return !(lhs < rhs);
    }
    inline friend bool operator<=(StaticString const& lhs, StaticString const& rhs) noexcept {
        return !(lhs > rhs);
    }

private:
    const_pointer m_string = nullptr;
    size_type m_length = 0;
    size_type m_hash = 0;
};


class CString {
    struct Data {
        uint32_t length;
    };
    union {
        char* m_cstr = nullptr;
        Data* m_data; // Data is stored at m_data[-1]
    };

public:
    using value_type      = char;
    using size_type       = uint32_t;
    using difference_type = int32_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = value_type*;
    using const_iterator  = const value_type*;

    CString() noexcept = default;
    CString(const char* cstr, size_type length);
    template<size_t N>
    explicit CString(StringLiteral<N> const& other) noexcept
            : CString(other, N - 1) {}
    CString(StaticString const& s) 
        : CString(s.c_str(), s.size()) {}
    CString(const CString& rhs);
    CString(CString&& rhs) noexcept { this->swap(rhs); }
    explicit CString(const char* cstr);
    CString& operator=(const CString& rhs);
    CString& operator=(CString&& rhs) noexcept {
        this->swap(rhs);
        return *this;
    }
    ~CString() noexcept {
        if (m_data) {
            free(m_data - 1);
        }
    }

    inline void swap(CString& other) noexcept {
        auto temp = m_cstr;
        m_cstr = other.m_cstr;
        other.m_cstr = temp;
    }
    inline const_pointer c_str() const noexcept { return m_cstr; }
    inline pointer c_str() noexcept { return m_cstr; }
    inline const_pointer c_str_safe() const noexcept { return m_data ? c_str() : ""; }
    inline const_pointer data() const noexcept { return c_str(); }
    inline pointer data() noexcept { return c_str(); }
    inline size_type size() const noexcept { return m_data ? m_data[-1].length : 0; }
    inline size_type length() const noexcept { return size(); }
    inline bool empty() const noexcept { return size() == 0; }

    inline iterator begin() noexcept { return m_cstr; }
    inline iterator end() noexcept { return begin() + length(); }
    inline const_iterator begin() const noexcept { return data(); }
    inline const_iterator end() const noexcept { return begin() + length(); }
    inline const_iterator cbegin() const noexcept { return begin(); }
    inline const_iterator cend() const noexcept { return end(); }

    inline const_reference operator[](size_type pos) const noexcept {
        assert(pos < size());
        return begin()[pos];
    }

    inline reference operator[](size_type pos) noexcept {
        assert(pos < size());
        return begin()[pos];
    }

    inline const_reference at(size_type pos) const noexcept {
        assert(pos < size());
        return begin()[pos];
    }

    inline reference at(size_type pos) noexcept {
        assert(pos < size());
        return begin()[pos];
    }

    inline reference front() noexcept {
        assert(size());
        return begin()[0];
    }

    inline const_reference front() const noexcept {
        assert(size());
        return begin()[0];
    }

    inline reference back() noexcept {
        assert(size());
        return begin()[size() - 1];
    }

    inline const_reference back() const noexcept {
        assert(size());
        return begin()[size() - 1];
    }

    // placement new declared as "throw" to avoid the compiler's null-check
    inline void* operator new(size_t size, void* ptr) {
        assert(ptr);
        return ptr;
    }

private:
    inline int compare(const CString& rhs) const noexcept {
        size_type lhs_size = size();
        size_type rhs_size = rhs.size();
        if (lhs_size < rhs_size) return -1;
        if (lhs_size > rhs_size) return 1;
        return strncmp(data(), rhs.data(), size());
    }

    inline friend bool operator==(CString const& lhs, StaticString const& rhs) noexcept {
        return (lhs.data() == rhs.data()) || ((lhs.size() == rhs.size()) && !strncmp(lhs.data(), rhs.data(), lhs.size()));
    }
    inline friend bool operator==(CString const& lhs, CString const& rhs) noexcept {
        return (lhs.data() == rhs.data()) || ((lhs.size() == rhs.size()) && !strncmp(lhs.data(), rhs.data(), lhs.size()));
    }
    inline friend bool operator!=(CString const& lhs, CString const& rhs) noexcept {
        return !(lhs == rhs);
    }
    inline friend bool operator<(CString const& lhs, CString const& rhs) noexcept {
        return lhs.compare(rhs) < 0;
    }
    inline friend bool operator>(CString const& lhs, CString const& rhs) noexcept {
        return lhs.compare(rhs) > 0;
    }
    inline friend bool operator>=(CString const& lhs, CString const& rhs) noexcept {
        return !(lhs < rhs);
    }
    inline friend bool operator<=(CString const& lhs, CString const& rhs) noexcept {
        return !(lhs > rhs);
    }
};

} // namespace utils

namespace std {

template<>
struct hash<utils::CString> {
    typedef utils::CString argument_type;
    typedef size_t result_type;
    utils::HashCStrings hasher;

    inline
    size_t 
    operator()(const utils::CString& s) const noexcept {
        return hasher(s.c_str());
    }
};

template<>
struct hash<utils::StaticString> {
    typedef utils::StaticString argument_type;
    typedef size_t result_type;

    inline
    size_t 
    operator()(const utils::StaticString& s) const noexcept {
        return s.hash();
    }
};

} // namespace std

#endif 
