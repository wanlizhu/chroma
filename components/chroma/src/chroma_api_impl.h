#ifndef CHROMA_CHROMA_API_IMPL_H
#define CHROMA_CHROMA_API_IMPL_H

/*
 * This looks like a header file, but really it acts as a .cpp, because it's used to
 * explicitly instantiate the methods of Builder<>
 */

#include <chroma/chroma_api.h>
#include <utility>

namespace chroma {
    
    template<typename T>
    Builder<T>::Builder() noexcept
            : mImpl(new T) {
    }

    template<typename T>
    template<typename ... ARGS>
    Builder<T>::Builder(ARGS&& ... args) noexcept
            : mImpl(new T(std::forward<ARGS>(args)...)) {
    }

    template<typename T>
    Builder<T>::~Builder() noexcept {
        delete mImpl;
    }

    template<typename T>
    Builder<T>::Builder(Builder const& rhs) noexcept
            : mImpl(new T(*rhs.mImpl)) {
    }

    template<typename T>
    Builder<T>& Builder<T>::operator=(Builder<T> const& rhs) noexcept {
        *mImpl = *rhs.mImpl;
        return *this;
    }
}

#endif
