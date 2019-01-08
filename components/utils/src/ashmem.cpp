#include <utils/ashmem.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#   include <fcntl.h>
#   include <stdlib.h>
#   include <unistd.h>
#endif

#if defined(WIN32) || defined (_WIN32)
#include <io.h>
#include <Windows.h>
#include <Winbase.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#endif


#ifdef ANDROID
#   include <sys/ioctl.h>
#   include <sys/stat.h>
#   include <memory.h>
#   include <linux/ashmem.h>
#   define ASHMEM_DEVICE "/dev/ashmem"
#endif

namespace utils {

#ifdef ANDROID

static int __ashmem_open() {
    int ret;
    struct stat st;

    int fd = open(ASHMEM_DEVICE, O_RDWR);
    if (fd < 0) {
        return fd;
    }

    ret = fstat(fd, &st);
    if (ret < 0) {
        int save_errno = errno;
        close(fd);
        errno = save_errno;
        return ret;
    }
    if (!S_ISCHR(st.st_mode) || !st.st_rdev) {
        close(fd);
        errno = ENOTTY;
        return -1;
    }

    return fd;
}

int ashmem_create_region(const char *name, size_t size) {
    int ret, save_errno;

    int fd = __ashmem_open();
    if (fd < 0) {
        return fd;
    }

    if (name) {
        char buf[ASHMEM_NAME_LEN] = {0};

        strlcpy(buf, name, sizeof(buf));
        ret = ioctl(fd, ASHMEM_SET_NAME, buf);
        if (ret < 0) {
            goto error;
        }
    }

    ret = ioctl(fd, ASHMEM_SET_SIZE, size);
    if (ret < 0) {
        goto error;
    }

    return fd;

error:
    save_errno = errno;
    close(fd);
    errno = save_errno;
    return ret;
}

#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

int ashmem_create_region(const char*, size_t size) {
    char template_path[512];
    snprintf(template_path, sizeof(template_path), "/tmp/chroma-ashmem-%d-XXXXXXXXX", getpid());
    int fd = mkstemp(template_path);
    if (fd == -1) return -1;
    unlink(template_path);
    if (ftruncate(fd, (off_t)size) == -1) {
        close(fd);
        return -1;
    }
    return fd;
}

#else
int ashmem_create_region(const char*, size_t size) {
    char template_path[512];
    snprintf(template_path, sizeof(template_path), "/tmp/chroma-ashmem-%lu-XXXXXXXXX", 
            GetCurrentProcessId());
    const char* tmp_path = _mktemp(template_path);
    int fd = _open(tmp_path, _O_BINARY);
    if (fd == -1) return -1;
    unlink(template_path);
    if (_chsize(fd, size) == -1) {
        _close(fd);
        return -1;
    }
    return fd;
}
#endif

} // namespace utils
