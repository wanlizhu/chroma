#ifndef CHROMA_RESC_LEXEME_H
#define CHROMA_RESC_LEXEME_H

#include <cstddef>
#include <string>
#include <vector>

namespace resc {

template<typename T>
class Lexeme {
public:
    Lexeme(T type,
           const char* start_, const char* end_
           size_t line_, size_t pos_)
        : m_start(start_)
        , m_end(end_)
        , m_line(line_)
        , m_pos(pos_) 
    {}
    
    inline const char* start() const noexcept { return m_start; }
    inline const char* end() const noexcept { return m_end; }
    inline size_t line() const noexcept { return m_line; }
    inline size_t line_pos() const noexcept { return m_pos; }
    inline size_t size() const noexcept { return m_end - m_start + 1; }
    inline const T type() const noexcept { return m_type; }
    inline const std::string value() const noexcept { return std::string(m_start, size()); }

protected:
    T m_type;
    const char* m_start;
    const char* m_end;
    size_t m_line;
    size_t m_pos;
};

}

#endif