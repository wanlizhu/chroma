#ifndef CHROMA_UTILS_PATH_H
#define CHROMA_UTILS_PATH_H

#include <ostream>
#include <string>
#include <vector>

namespace utils {

class Path {
public:
    static std::string canonical(const std::string& pathname);
    static Path concat(const std::string& root, const std::string& leaf);
    static Path pwd();
    static Path current_executable();

    Path() = default;
    Path(const char* pathname);
    Path(const std::string& pathname);
    ~Path() = default;

    bool exists() const;
    bool is_file() const;
    bool is_directory() const;
    inline bool is_empty() const { return m_path.empty(); }
    inline const char* c_str() const { return m_path.c_str(); }

    inline void set(const std::string& pathname) noexcept { m_path = canonical(pathname); }
    inline const std::string& get() const noexcept { return m_path; }
    Path parent() const;
    Path ancestor(int n) const;

    std::string name() const;
    std::string name_without_ext() const;
    std::string extension() const;

    Path absolute_path() const;
    bool is_absolute() const;

    std::vector<std::string> split() const;
    Path concat(const Path& path) const;
    void concat_to_self(const Path& path);

    inline operator std::string const&() const noexcept { return m_path; }
    inline Path operator+(const Path& rhs) const noexcept { return concat(rhs); }
    inline Path& operator+=(const Path& rhs) noexcept {
        concat_to_self(rhs);
        return *this;
    }
    inline bool operator==(const Path& rhs) const { return m_path == rhs.m_path; }
    inline bool operator!=(const Path& rhs) const { return m_path != rhs.m_path; }
    inline bool operator<(const Path& rhs) const { return m_path < rhs.m_path; }
    inline bool operator>(const Path& rhs) const { return m_path > rhs.m_path; }
    friend std::ostream& operator<<(std::ostream& os, const Path& path);

    bool mkdir() const;
    bool mkdirs() const;
    bool unlink_file();
    std::vector<Path> list() const;

private:
    std::string m_path;
};

} // namespace utils

#endif 
