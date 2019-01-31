#ifndef CHROMA_SYS_C_STRING_H
#define CHROMA_SYS_C_STRING_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <array>
#include <functional>
#include <string>
#include <algorithm>
#include <vector>
#include <system/deleter.h>

namespace sys {

char** cstr_array(const std::vector<std::string>& strs, StringArrayDeleter* del = nullptr);

}
#endif 
