#ifndef CHROMA_SYSTEM_CONTAINER_H
#define CHROMA_SYSTEM_CONTAINER_H

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <type_traits>

namespace sys {

struct not_exist {};

template<typename COND,
         template<typename, typename> typename V,
         typename T, 
         typename A>
inline void push_back_if(V<T, A>& container, const T& value) {
    if (COND::value) {
        container.push_back(value);
    }
}

template<template<typename, typename> typename V,
         typename T,
         typename A>
inline void push_back_if<not_exist, V<T, A>, T, A>(V<T, A>& container, const T& value) {
    if (std::find(container.begin(), container.end(), value) != container.end()) {
        container.push_back(value);
    }
}

template<template<typename, typename> typename V,
         typename T,
         typename A>
inline V<T, A> distinct(const V<T, A>& vec) {
    V<T, A> res(vec);

    std::sort(res.begin(), res.end());
    typename V<T, A>::iterator uniq = std::unique(res.begin(), res.end());
    res.erase(uniq, res.end());

    return res;
}

template<typename T>
inline bool is_subset_of(const T& sub, const T& main) {
    if (main.size() < sub.size()) {
        return false;
    }

    for (size_t i = 0; i < sub.size(); i++) {
        if (std::find(main.begin(), main.end(), sub[i]) == main.end()) {
            return false;
        }
    }

    return true;
}

} // namespace sys

#endif