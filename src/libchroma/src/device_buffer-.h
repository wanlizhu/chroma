#ifndef CHROMA_VERTEX_BUFFER_H
#define CHROMA_VERTEX_BUFFER_H

#include "idevice_buffer.h"

namespace chroma { namespace device {

class StreamedBuffer 
    : public IDeviceBuffer
    , public std::enable_shared_from_this<VertexBuffer> {
public:
    VertexBuffer(const void* data_, size_t size_);
    virtual ~VertexBuffer();

    virtual size_t size() const noexcept override;
    virtual IDeviceBuffer::Type type() const noexcept override;
    size_t capacity() const noexcept;
    bool is_mapped() const noexcept;
    void* map(Map opt) const;
    void unmap() const;
    void append(const void* data_, size_t size_);
    void resize(size_t size_);
    void reserve(size_t capa_);

private:
    size_t m_size = 0;
    size_t m_capacity = 0;
    void* m_mapped = nullptr;
};

}} // namespace chroma -> device

#endif