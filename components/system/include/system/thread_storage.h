#ifndef CHROMA_THREAD_STORAGE_H
#define CHROMA_THREAD_STORAGE_H

#include <unordered_map> 
#include <thread> 
#include <mutex> 
#include <shared_mutex> 
#include <utility>
#include <vector> 
#include <memory> 
#include <algorithm> 

namespace sys {

struct DynamicStorageBase {
    virtual ~DynamicStorageBase() noexcept = default;
};

template<typename T>
struct ValueStorage : public DynamicStorageBase {
    template<typename... Args>
    ValueStorage(Args&&... args) :
        value(std::forward<Args>(args)...) {}

    T value;
};

template<typename T>
class ThreadStorage {
public:
    unsigned int add(T** obj = nullptr);

    T* get(unsigned int id);
    const T* get(unsigned int id) const;
    bool remove(unsigned int id);

protected:
    unsigned int highestID_{};
    std::vector<unsigned int> ids_;
    std::unordered_map<std::thread::id, std::unordered_map<unsigned int, T>> objects_;
    mutable std::shared_mutex mutex_;
};

using DynamicStoragePtr = std::unique_ptr<DynamicStorageBase>;
using DynamicThreadStorage = ThreadStorage<DynamicStoragePtr>;













template<typename T>
unsigned int ThreadStorage<T>::add(T** obj) {
    std::scoped_lock lock(mutex_);
    auto id = ++highestID_;
    ids_.push_back(id);
    if (obj) {
        *obj = &objects_[std::this_thread::get_id()][id];
    }

    return id;
}

template<typename T>
T* ThreadStorage<T>::get(unsigned int id) {
    std::shared_lock lock(mutex_);
    if (std::find(ids_.begin(), ids_.end(), id) == ids_.end()) {
        return nullptr;
    }

    return &objects_[std::this_thread::get_id()][id];
}

template<typename T>
const T* ThreadStorage<T>::get(unsigned int id) const {
    std::shared_lock lock(mutex_);
    if (std::find(ids_.begin(), ids_.end(), id) == ids_.end()) {
        return nullptr;
    }

    return &objects_[std::this_thread::get_id()][id];
}

template<typename T>
bool ThreadStorage<T>::remove(unsigned int id) {
    std::scoped_lock lock(mutex_);
    auto it = std::find(ids_.begin(), ids_.end(), id);
    if (it == ids_.end()) {
        return false;
    }

    ids_.erase(it);
    for (auto& obj : objects_) {
        obj.second.erase(id);
    }

    return true;
}

} // namespace sys

#endif