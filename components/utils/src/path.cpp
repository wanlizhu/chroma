#include <utils/path.h>
#include <sstream>
#include <ostream>
#include <iterator>
#include <regex>
#include <limits.h>
#include <vector>
#include <sys/stat.h>

#if defined(WIN32)
#   include <utils/compiler.h>
#   include <direct.h>
#   include <Strsafe.h>
#   include <stdlib.h>
#   include <windows.h>
#   include <shlwapi.h>
#   include "stdtypes.h"
#   define SEPARATOR '\\'
#   define SEPARATOR_STR  "\\"
#else
#   include <unistd.h>
#   include <dirent.h>
#   define SEPARATOR '/'
#   define SEPARATOR_STR "/"
#   ifdef APPLE
#       include <mach-o/dyld.h>
#   endif
#endif

namespace utils {

Path::Path(const char* path)
    : Path(std::string(path)) {
}

Path::Path(const std::string& path)
    : m_path(canonical(path)) {
}

bool Path::exists() const {
    struct stat file;
    return stat(c_str(), &file) == 0;
}

bool Path::is_file() const {
    struct stat file;
    if (stat(c_str(), &file) == 0) {
        return S_ISREG(file.st_mode);
    }
    return false;
}

bool Path::is_directory() const {
    struct stat file;
    if (stat(c_str(), &file) == 0) {
        return S_ISDIR(file.st_mode);
    }
    return false;
}

Path Path::concat(const Path& path) const {
    if (path.is_empty()) return *this;
    if (path.is_absolute()) return path;
    if (!m_path.empty() && m_path.back() != SEPARATOR) {
        return Path(m_path + SEPARATOR + path.get());
    }
    return Path(m_path + path.get());
}

void Path::concat_to_self(const Path& path)  {
    if (!path.is_empty()) {
        if (path.is_absolute()) {
            m_path = path.get();
        } 
        else if (!m_path.empty() && m_path.back() != SEPARATOR) {
            m_path = canonical(m_path + SEPARATOR + path.get());
        } 
        else {
            m_path = canonical(m_path + path.get());
        }
    }
}

Path Path::concat(const std::string& root, const std::string& leaf) {
    return Path(root).concat(Path(leaf));
}

Path Path::pwd() {
    char directory[PATH_MAX + 1];
    if (getcwd(directory, PATH_MAX) == nullptr) {
        return Path();
    }
    return Path(directory);
}

Path Path::absolute_path() const {
    if (is_empty() || is_absolute()) {
        return *this;
    }
    return pwd().concat(*this);
}


#if !defined(WIN32)
bool Path::is_absolute() const {
    return !is_empty() && m_path.front() == '/';
}
#endif

Path Path::parent() const {
    if (is_empty()) return "";

    std::string result;

    std::vector<std::string> segments(split());

    // if our path is absolute with a single segment,
    // be sure to keep the prefix component
    if (!is_absolute() || segments.size() > 1) {
        segments.pop_back(); // peel the last one
    }

    for (auto const& s : segments) {
        result.append(s).append(SEPARATOR_STR);
    }
    return canonical(result);
}

Path Path::ancestor(int n) const {
    Path result = parent();
    while (n--) {
        result = result.parent();
    }
    return result;
}

std::string Path::name() const {
    if (is_empty()) return "";

    std::vector<std::string> segments(split());
    return segments.back();
}

std::string Path::extension() const {
    if (is_empty() || is_directory()) {
        return "";
    }

    auto name_ = name();
    auto index = name_.rfind('.');
    if (index != std::string::npos && index != 0) {
        return name_.substr(index + 1);
    } 
    else {
        return "";
    }
}

std::string Path::name_without_ext() const {
    std::string name_ = name();
    size_t index = name_.rfind('.');
    if (index != std::string::npos) {
        return name_.substr(0, index);
    }
    return name_;
}

std::ostream& operator<<(std::ostream& os, const Path& path) {
    os << path.get();
    return os;
}

std::vector<std::string> Path::split() const {
    std::vector<std::string> segments;
    if (is_empty()) return segments;

    size_t current;
    ssize_t next = -1;

    // Matches a leading disk designator (C:\), forward slash (/), or back slash (\)
    const static std::regex driveDesignationRegex(R"_regex(^([a-zA-Z]:\\|\\|\/))_regex");
    std::smatch match;
    if (std::regex_search(m_path, match, driveDesignationRegex)) {
        segments.push_back(match[0]);
        next = match[0].length() - 1;
    }

    do {
      current = size_t(next + 1);
      next = m_path.find_first_of(SEPARATOR_STR, current);

      std::string segment(m_path.substr(current, next - current));
      if (!segment.empty()) segments.push_back(segment);
    } while (next != std::string::npos);

    if (segments.empty()) segments.push_back(m_path);

    return segments;
}

std::string Path::canonical(const std::string& path) {
    if (path.empty()) return "";

    std::vector<std::string> segments;

    // If the path starts with a / we must preserve it
    bool starts_with_slash = path.front() == SEPARATOR;
    // If the path does not end with a / we need to remove the
    // extra / added by the join process
    bool ends_with_slash = path.back() == SEPARATOR;

    size_t current;
    ssize_t next = -1;

    do {
        current = size_t(next + 1);
        // Handle both Unix and Windows style separators
        next = path.find_first_of("/\\", current);

        std::string segment(path.substr(current, next - current));
        size_t size = segment.length();

        // skip empty (keedp initial)
        if (size == 0 && !segments.empty()) {
            continue;
        }

        // skip . (keep initial)
        if (segment == "." && !segments.empty()) {
            continue;
        }

        // remove ..
        if (segment == ".." && !segments.empty()) {
            if (segments.back().empty()) { // ignore if .. follows initial /
                continue;
            }
            if (segments.back() != "..") {
                segments.pop_back();
                continue;
            }
        }

        segments.push_back(segment);
    } while (next != std::string::npos);

    // Join the vector as a single string, every element is
    // separated by a '/'. This process adds an extra / at
    // the end that might need to be removed
    std::stringstream clean_path;
    std::copy(segments.begin(), segments.end(),
            std::ostream_iterator<std::string>(clean_path, SEPARATOR_STR));
    std::string new_path = clean_path.str();

    if (starts_with_slash && new_path.empty()) {
        new_path = SEPARATOR_STR;
    }

    if (!ends_with_slash && new_path.length() > 1) {
        new_path.pop_back();
    }

    return new_path;
}

bool Path::mkdirs() const {
    if (is_empty()) {
        return true;
    }
    errno = 0;
    bool success = mkdir();
    if (!success) {
        int save_errno = errno;
        switch (save_errno) {
        case EEXIST: {
            bool result = is_directory();
            errno = save_errno;
            return result;
        }
        case ENOENT:
            parent().mkdirs();
            return mkdir();
        default:
            break;
        }
    }
    return success;
}

bool Path::unlink_file() {
    return ::unlink(m_path.c_str()) == 0;
}

#ifdef WIN32

bool Path::mkdir() const {
    return _mkdir(m_path.c_str()) == 0;     
}

Path Path::current_executable() {
    // First, need to establish resource path.
    TCHAR path[MAX_PATH + 1];
    Path result;

    GetModuleFileName(NULL, path, MAX_PATH + 1);
    result.set(path);

    return result;
}

std::vector<Path> Path::list() const {
    // Return an empty vector if the path doesn't exist or is not a directory
    if (!is_directory() || !exists()) {
        return {};
    }

    TCHAR dirName[MAX_PATH];
    StringCchCopy(dirName, MAX_PATH, c_str());

    WIN32_FIND_DATA findData;
    HANDLE find = FindFirstFile(dirName, &findData);

    std::vector<Path> directory_contents;
    do
    {
        if (findData.cFileName[0] != '.') {
            directory_contents.push_back(concat(findData.cFileName));
        }
    } while (FindNextFile(find, &findData) != 0);

    return directory_contents;
}

bool Path::is_absolute() const {
    return !PathIsRelative(m_path.c_str());
}

#elif defined(LINUX)

bool Path::mkdir() const {
    return ::mkdir(m_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
}

Path Path::current_executable() {
    // First, need to establish resource path.
    char exec_buf[2048];
    Path result;

    uint32_t buffer_size = sizeof(exec_buf)-1;
    ssize_t sz = readlink("/proc/self/exe", exec_buf, buffer_size);
    if (sz > 0) {
        exec_buf[sz] = 0;
        result.set(exec_buf);
    }

    return result;
}

std::vector<Path> Path::list() const {
    // Return an empty vector if the path doesn't exist or is not a directory
    if (!is_directory() || !exists()) {
        return {};
    }

    struct dirent* directory;
    DIR* dir;

    dir = opendir(c_str());
    if (dir == nullptr) {
        // Path does not exist or could not be read
        return {};
    }

    std::vector<Path> directory_contents;

    while ((directory = readdir(dir)) != nullptr) {
        const char* file = directory->d_name;
        if (file[0] != '.') {
            directory_contents.push_back(concat(directory->d_name));
        }
    }

    closedir(dir);
    return directory_contents;
}

#elif defined(APPLE)

bool Path::mkdir() const {
    return ::mkdir(m_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
}

Path Path::current_executable() {
    // First, need to establish resource path.
    char exec_buf[2048];
    Path result;

    uint32_t buffer_size = sizeof(exec_buf);
    if (_NSGetExecutablePath(exec_buf, &buffer_size) == 0) {
        result.set(exec_buf);
    }

    return result;
}

std::vector<Path> Path::list() const {
    // Return an empty vector if the path doesn't exist or is not a directory
    if (!is_directory() || !exists()) {
        return {};
    }

    struct dirent* directory;
    DIR* dir;

    dir = opendir(c_str());
    if (dir == nullptr) {
        // Path does not exist or could not be read
        return {};
    }

    std::vector<Path> directory_contents;

    while ((directory = readdir(dir)) != nullptr) {
        const char* file = directory->d_name;
        if (file[0] != '.') {
            directory_contents.push_back(concat(directory->d_name));
        }
    }

    closedir(dir);
    return directory_contents;
}

#endif

} // namespace utils
