#ifndef CHROMA_UTILS_PATH_H
#define CHROMA_UTILS_PATH_H

#include <ostream>
#include <string>
#include <vector>

namespace utils {

class Path {
public:
    Path() = default;
    Path(const char* pathname);
    Path(const std::string& pathname);
    ~Path() = default;

    bool exists() const;
    bool is_file() const;
    bool is_directory() const;
    bool is_empty() const { return m_path.empty(); }
    const char* c_str() const { return m_path.c_str(); }

    void set(const std::string& pathname) {
        m_path = canonical(pathname);
    }
    const std::string& get() const { return m_path; }
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

    operator std::string const&() const { return m_path; }
    Path operator+(const Path& rhs) const { return concat(rhs); }
    Path& operator+=(const Path& rhs) {
        concat_to_self(rhs);
        return *this;
    }
    bool operator==(const Path& rhs) const { return m_path == rhs.m_path; }
    bool operator!=(const Path& rhs) const { return m_path != rhs.m_path; }
    bool operator<(const Path& rhs) const { return m_path < rhs.m_path; }
    bool operator>(const Path& rhs) const { return m_path > rhs.m_path; }
    friend std::ostream& operator<<(std::ostream& os, const Path& path);

    static std::string canonical(const std::string& pathname);
    static Path concat(const std::string& root, const std::string& leaf);

    static Path pwd();
    static Path current_executable();

    bool mkdir() const;
    bool mkdirs() const;
    bool unlink_file();
    std::vector<Path> list_contents() const;

private:
    std::string m_path;
};

} // namespace utils

#endif 
