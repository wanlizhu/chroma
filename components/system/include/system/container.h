#ifndef CHROMA_SYSTEM_CONTAINER_H
#define CHROMA_SYSTEM_CONTAINER_H

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <type_traits>

namespace sys {

struct not_exist {};

template<typename PREDICATE, typename CONTAINER>
void push_back_if(CONTAINER& container, const CONTAINER::value_type& value) {
    if (PREDICATE::value) {
        container.push_back(value);
    }
}

template<typename CONTAINER>
void push_back_if<not_exist, CONTAINER>(CONTAINER& container, const CONTAINER::value_type& value) {
    if (std::find(container.begin(), container.end(), value) != container.end()) {
        container.push_back(value);
    }
}

} // namespace sys

#endif