#ifndef CHROMA_RESC_LEXER_H
#define CHROMA_RESC_LEXER_H

#include <cstddef>
#include "lexeme.h"

namespace resc {

template<typename T>
class Lexer {
public:
    virtual ~Lexer() = default;

    void lex(const char* text, size_t size, size_t offset = 1) {
        m_cursor = text;
        m_end = m_cursor + size;
        m_line_counter = offset;
        while (has_more()) {
            if (!read_lexeme()) {
                return;
            }
        }
    }

    inline const std::vector<T>& lexemes() const noexcept { return m_lexemes; }

protected:
    virtual bool read_lexeme() noexcept = 0;
    inline size_t line() const noexcept { return m_line_counter; }
    inline size_t cursor() const noexcept { return m_line_pos; }
    void skip_whitespace() {
        while (m_cursor < m_end && is_whitespace(*m_cursor)) {
            consume();
        }

        if (has_more() && *m_cursor == '/' && m_cursor < (m_end - 1) && *(m_cursor + 1) == '/') {
            skip_until_eol();
            skip_whitespace();
        }
    }
    
    inline bool is_whitespace(char c) const noexcept {
        return c < 33 || c > 127;
    }

    inline bool is_alphabet(char c) const noexcept {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    inline bool is_numeric(char c) const noexcept {
        return (c >= '0' && c <= '9');
    }

    inline bool is_numeric_apha(char c) const noexcept {
        return  is_alphabet(c) || is_numeric(c);
    }

    inline bool is_identifier(char c) const noexcept {
        return is_numeric_apha(c) || (c == '_');
    }

    inline bool has_more() const noexcept {
        return m_cursor < m_end;
    }

    inline void skip_until_eol() {
        while (*m_cursor != '\n' && has_more()) {
            consume();
        }
    }

    inline char consume() noexcept {
        if (*m_cursor == '\n') {
            m_line_counter++;
            m_line_pos = 0;
        }
        m_line_pos++;
        return *m_cursor++;
    }

    inline char peek() const noexcept {
        return *m_cursor;
    }

protected:
    const char* m_end = nullptr;
    const char* m_cursor = nullptr;
    std::vector<T> m_lexemes;

    size_t m_line_counter = 1;
    size_t m_line_pos = 0;
};

}

#endif