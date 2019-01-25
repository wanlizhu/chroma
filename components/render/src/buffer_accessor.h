#ifndef CHROMA_BUFFER_ACCESSOR_H
#define CHROMA_BUFFER_ACCESSOR_H

#include <type_traits>
#include <assert.h>

namespace render { namespace device {

template<typename ELEMENT, int COMPONENTS, int STRIDE>
class BufferAccessor {
public:
    enum Attrib {
        Components = COMPONENTS,
        Stride = (STRIDE == 0 ? sizeof(ELEMENT) : STRIDE),
    };

    BufferAccessor(IBuffer* buffer, size_t offset_, size_t size)
        : m_buffer(buffer)
        , m_offset(offset_)
        , m_size(size) 
        , m_current(nullptr)
        , m_end(nullptr) {
        m_current = move_forward(m_buffer->begin(), m_offset);
        m_end = move_forward(m_current, m_size);
        assert(m_current <= m_end);
    }
    virtual ~BufferAccessor() = default;

    inline ELEMENT* current() noexcept { return m_current; }
    inline ELEMENT* end() noexcept { return m_end; }
    inline size_t size() noexcept { return m_size; }
    inline bool next() noexcept {
        assert(m_current != nullptr);
        m_current = move_forward(m_current, Attrib::Stride);
        return (m_current < m_end);
    }

protected:
    void* move_forward(const void* ptr, size_t size) {
        return static_cast<void*>(reinterpret_cast<char*>(ptr) + size);
    }

protected:
    IBuffer* m_buffer = nullptr;
    size_t m_offset = 0;
    size_t m_size = 0;
    ELEMENT* m_current = nullptr;
    const void* m_end = nullptr;
};

}} // namespace render -> device

#endif