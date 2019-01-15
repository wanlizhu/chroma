#include <utils/cstring.h>
#include <utils/compiler.h>
#include <memory>

namespace utils {

HashCStrings::result_type HashCStrings::operator()(argument_type cstr) const noexcept {
    size_t hash = 5381;
    while (int c = *cstr++) {
        hash = (hash * 33u) ^ size_t(c);
    }
    return hash;
}

bool EqualCStrings::operator()(const char* lhs, const char* rhs) const noexcept {
    return !strcmp(lhs, rhs);
}

LessCStrings::result_type LessCStrings::operator()(first_argument_type lhs,
                                                   second_argument_type rhs) const noexcept {
    return strcmp(lhs, rhs) < 0;
}

constexpr StaticString StaticString::make(const_pointer literal, size_t length) noexcept {
    StaticString r;
    r.m_string = literal;
    r.m_length = size_type(length);
    size_type hash = 5381;
    while (int c = *literal++) {
        hash = (hash * 33u) ^ size_type(c);
    }
    r.m_hash = hash;
    return r;
}

StaticString StaticString::make(const_pointer literal) noexcept {
    return make(literal, strlen(literal));
}

int StaticString::compare(const StaticString& rhs) const noexcept {
    size_type lhs_size = size();
    size_type rhs_size = rhs.size();
    if (lhs_size < rhs_size) return -1;
    if (lhs_size > rhs_size) return 1;
    return strncmp(data(), rhs.data(), size());
}

//UTILS_NOINLINE
CString::CString(const char* cstr, size_type length) {
    if (length && cstr) {
        // assert(length == strlen(cstr));
        Data* p = (Data*)malloc(sizeof(Data) + length + 1);
        p->length = length;
        m_cstr = (value_type*)(p + 1);
        // we don't use memcpy here to avoid a call to libc, the generated code is pretty good.
        std::uninitialized_copy_n(cstr, length + 1, m_cstr);
    }
}

CString::CString(const char* cstr)
        : CString(cstr, size_type(cstr ? strlen(cstr) : 0)) {
}

CString::CString(const CString& rhs)
        : CString(rhs.c_str(), rhs.size()) {
}

CString& CString::operator=(const CString& rhs) {
    if (this != &rhs) {
        auto const p = m_data ? m_data - 1 : nullptr;
        new(this) CString(rhs);
        free(p);
    }
    return *this;
}

} // namespace utils
