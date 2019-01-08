#if defined(__APPLE__)

#include <utils/path.h>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <mach-o/dyld.h>

namespace utils {

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

std::vector<Path> Path::list_contents() const {
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

} // namespace utils

#endif