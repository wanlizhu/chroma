#include <system/c_str.h>
#include <system/compiler.h>
#include <memory>

namespace sys {

char** cstr_array(const std::vector<std::string>& strs, StringArrayDeleter* del) {
    char** cstrs = new char*[strs.size()];
    size_t strsize = 0;
    for (size_t i = 0; i < strs.size(); i++) {
        strsize = strs[i].size() + 1;
        cstrs[i] = new char[strsize];
        cstrs[i][strsize - 1] = '\0';
        memcpy(cstrs[i], strs[i].c_str(), strsize);
    }

    if (del)
        del->add((const char**)cstrs, strs.size());

    return cstrs;
}

} // namespace sys
