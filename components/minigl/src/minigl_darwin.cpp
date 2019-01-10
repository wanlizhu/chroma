#include <dlfcn.h>

namespace minigl {

static void* library = nullptr;

bool init_binder() {
    const char* library_name = "/System/Library/Frameworks/OpenGL.framework/OpenGL";
    library = dlopen(library_name, RTLD_GLOBAL | RTLD_NOW);
    return library != nullptr;
}

void* load_function(const char* name) {
    return dlsym(library, name);
}

void shutdown_binder() {
    dlclose(library);
    library = nullptr;
}

} 
