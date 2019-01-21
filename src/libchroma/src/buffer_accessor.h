#ifndef CHROMA_BUFFER_ACCESSOR_H
#define CHROMA_BUFFER_ACCESSOR_H

#include "idevice_buffer.h"
#include <type_traits>

namespace chroma { namespace device {

template<typename ELEMENT, int COMPONENTS_, int STRIDE_,
         typename = std::enable_if<std::is_integral<ELEMENT>::value 
                                || std::is_floating_point<ELEMENT>::value> >
class BufferAccessor {
public:
    enum Attrib {
        Components = COMPONENTS_,
        Stride = (STRIDE_ == 0 ? sizeof(ELEMENT) : STRIDE_),
    };

    BufferAccessor(IBuffer* buffer, size_t offset_, size_t size)
        : m_buffer(buffer)
        , m_offset(offset_)
        , m_size(size) 
        , m_prev(nullptr)
        , m_end(nullptr) {
        m_end = offset(m_buffer->begin(), m_offset + m_size);
    }
    virtual ~BufferAccessor() = default;

    inline ELEMENT* next() noexcept {
        if (m_prev == nullptr) {
            m_prev = (ELEMENT*)offset(m_buffer->begin(), m_offset);
            return m_prev;
        }

        m_prev = offset(m_prev, Attrib::Stride);
        return (m_prev < m_end ? m_prev : nullptr);
    }

protected:
    void* offset(const void* ptr, size_t size) {
        return static_cast<void*>(reinterpret_cast<char*>(ptr) + size);
    }

protected:
    IBuffer* m_buffer = nullptr;
    size_t m_offset = 0;
    size_t m_size = 0;
    ELEMENT* m_prev = nullptr;
    const void* m_end = nullptr;
};

}} // namespace chroma -> device

#endif