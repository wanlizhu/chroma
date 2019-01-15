#ifndef CHROMA_TOOLS_RESC_API_H
#define CHROMA_TOOLS_RESC_API_H

#include <stdio.h>
#include <vector>
#include <string>

namespace resc {
    
bool compile_shader(const char* src_file, const char* dst_str, const std::vector<std::string>& inc_paths);

}

#endif