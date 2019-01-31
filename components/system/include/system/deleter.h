#ifndef CHROMA_SYSTEM_DELETER_H
#define CHROMA_SYSTEM_DELETER_H

#include <iostream>
#include <functional>
#include <vector>
#include <type_traits>

namespace sys {

struct StringDeleter {
    const char* str = nullptr;
    StringDeleter* next = nullptr;
                          
    StringDeleter() = default;
    StringDeleter(const StringDeleter&) = delete;
    StringDeleter& operator=(const StringDeleter&) = delete;

    explicit StringDeleter(const char* ptr)
        : str(ptr)
        , next(nullptr)
    {}

    virtual ~StringDeleter() {
        if (next) delete next;
        if (str) delete[] str;
    }

    void add(const char* ptr) {
        if (next) next->add(ptr);
        else next = new StringDeleter(ptr);
    }
};

struct StringArrayDeleter {
    const char** strs = nullptr;
    size_t count = 0;
    StringArrayDeleter* next = nullptr;

    StringArrayDeleter() = default;
    StringArrayDeleter(const StringArrayDeleter&) = delete;
    StringArrayDeleter& operator=(const StringArrayDeleter&) = delete;

    explicit StringArrayDeleter(const char** ptr, size_t count_)
        : strs(ptr)
        , count(count_)
        , next(nullptr)
    {}

    virtual ~StringArrayDeleter() {
        if (next) delete next;
        if (strs) {
            for (size_t i = 0; i < count; i++) {
                delete[] strs[i];
                strs[i] = nullptr;
            }
            delete[] strs;
        }
    }

    void add(const char** ptr, size_t count_) {
        if (next) next->add(ptr, count_);
        else next = new StringArrayDeleter(ptr, count_);
    }
};

}

#endif