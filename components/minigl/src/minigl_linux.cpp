#include <GL/gl.h>
#include <dlfcn.h>
#include <string.h>

namespace minigl {

struct Driver {
    void* (*glXGetProcAddress)(const GLubyte*);
    void* library;
} g_driver = {nullptr, nullptr};

bool init_binder() {
    const char* library_name = "libGL.so.1";
    g_driver.library = dlopen(library_name, RTLD_GLOBAL | RTLD_NOW);

    if (!g_driver.library) {
        return false;
    }

    g_driver.glXGetProcAddress = (void *(*)(const GLubyte *))
            dlsym(g_driver.library, "glXGetProcAddressARB");

    return g_driver.glXGetProcAddress;
}

void* load_function(const char* name) {
    return (void*) g_driver.glXGetProcAddress((const GLubyte*) name);
}

void shutdown_binder() {
    dlclose(g_driver.library);
    memset(&g_driver, 0, sizeof(g_driver));
}

} 
