#if defined(WIN32)

#include <utils/path.h>
#include <direct.h>
#include <Strsafe.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <windows.h>
#include <shlwapi.h>

namespace utils {

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

std::vector<Path> Path::list_contents() const {
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

} // namespace utils

#endif