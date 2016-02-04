 #pragma once

#include "utils/BasicStorage.h"
#include "utils/Assert.h"
#include <limits>
#include <cstdlib>
#include <cstdint>
#include <utility>

namespace pg {

/*
 * An array whose elements are stored in contiguous blocks in memory.
 * Implements a ubset of std::vector features.
 */
template<typename T>
class DynamicArray {
public:
    explicit DynamicArray(std::size_t capacity);
    DynamicArray()                                  = default;
    DynamicArray(const DynamicArray&)               = delete;
    DynamicArray& operator=(const DynamicArray&)    = delete;
    DynamicArray(DynamicArray&&);
    DynamicArray& operator=(DynamicArray&&);
    ~DynamicArray();

    using Iterator = T*;

    struct ReverseIterator {
        ReverseIterator()                                   = delete;
        ReverseIterator(const ReverseIterator&)             = default;
        ReverseIterator(ReverseIterator&&)                  = default;
        ReverseIterator& operator=(const ReverseIterator&)  = default;
        ReverseIterator& operator=(ReverseIterator&&)       = default;
        ReverseIterator(DynamicArray<T>* owner, std::size_t start)
            : owner_{ owner },
            curIndex_{ start } {}
        ~ReverseIterator()                                  = default;

        bool operator==(const ReverseIterator& rhs) const {
            return owner_ == rhs.owner_ && curIndex_ == rhs.curIndex_;
        }

        bool operator!=(const ReverseIterator& rhs) const {
            return !(this->operator==(rhs));
        }

        ReverseIterator& operator++() {
            curIndex_ = curIndex_ - 1u;
            return *this;
        }

        ReverseIterator operator++(int) {
            ReverseIterator was{ *this };
            curIndex_ = curIndex_ - 1u;
            return was;
        }

        T& operator*() {
            return *owner_->storage_.at(curIndex_);
        }

        T* operator->() {
            return owner_->storage_.at(curIndex_);
        }

        operator std::size_t() const {
            return curIndex_;
        }

    private:
        DynamicArray<T>*    owner_;
        std::size_t         curIndex_;
    };

    // element access
    Iterator begin();
    Iterator end();
    ReverseIterator rbegin();
    ReverseIterator rend();
    T& at(std::size_t);
    const T& at(std::size_t) const;
    T& operator[](std::size_t);
    const T& operator[](std::size_t) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T*  data();

    // modifiers
    void pushBack(const T&);
    template<typename... Args>
    void emplaceBack(Args&&... args);
    void popBack();
    // Remove an element at index by swapping it with the last element and calling popBack().
    void swapAndRemove(std::size_t);
    void swapAndRemove(Iterator);
    void swapAndRemove(ReverseIterator);
    void clear();

    // container data
    std::size_t size() const;
    std::size_t capacity() const;

private:
    void ensureCapacity_();
    BasicStorage<T> storage_{0u};
    std::size_t     size_{0u};
};

template<typename T, std::size_t N>
class StaticArray {
public:

private:
    uint8_t storage_[sizeof(T) * N];
};

/***
*       ___                       _     ___
*      / _ \__ _____  ___ ___ _  (_)___/ _ | ___________ ___ __
*     / // / // / _ \/ _ `/  ' \/ / __/ __ |/ __/ __/ _ `/ // /
*    /____/\_, /_//_/\_,_/_/_/_/_/\__/_/ |_/_/ /_/  \_,_/\_, /
*         /___/                                         /___/
*/

template<typename T>
DynamicArray<T>::DynamicArray( std::size_t capacity)
:   storage_{capacity},
    size_{0u}
    {}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    clear();
}

template<typename T>
T& DynamicArray<T>::at(std::size_t i) {
    PG_ASSERT(i < size_);
    return *storage_.at(i);
}

template<typename T>
const T& DynamicArray<T>::at(std::size_t i) const {
    return *storage_.at(i);
}

template<typename T>
T& DynamicArray<T>::operator[](std::size_t i) {
    return at(i);
}

template<typename T>
const T& DynamicArray<T>::operator[](std::size_t i) const {
    return at(i);
}

template<typename T>
T& DynamicArray<T>::front() {
    PG_ASSERT(size_ != 0u);
    return at(0u);
}

template<typename T>
const T& DynamicArray<T>::front() const {
    PG_ASSERT(size_ != 0u);
    return at(0u);
}

template<typename T>
T& DynamicArray<T>::back() {
    PG_ASSERT(size_ != 0u);
    return at(size_ - 1u);
}

template<typename T>
const T& DynamicArray<T>::back() const {
    PG_ASSERT(size_ != 0u);
    return at(size_ - 1u);
}

template<typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::begin() {
    PG_ASSERT(storage_.capacity() != 0u);
    return storage_.at(0);
}

template<typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::end() {
    PG_ASSERT(storage_.capacity() != 0u);
    return storage_.at(0) + size_;
}

template<typename T>
typename DynamicArray<T>::ReverseIterator DynamicArray<T>::rbegin() {
    PG_ASSERT(storage_.capacity() != 0u);
    return ReverseIterator{ this, size_ - 1u };
}

template<typename T>
typename DynamicArray<T>::ReverseIterator DynamicArray<T>::rend() {
    PG_ASSERT(storage_.capacity() != 0u);
    return ReverseIterator{ this, std::numeric_limits<std::size_t>::max() };
}

template<typename T>
T* DynamicArray<T>::data() {
    PG_ASSERT(storage_.capacity() != 0u);
    return storage_.at(0);
}

template<typename T>
void DynamicArray<T>::ensureCapacity_() {
    if (storage_.capacity() == 0u) {
        storage_.resize(8u);
    }
    else if (size_ == storage_.capacity()) {
        storage_.resize(storage_.capacity() * 2u);
    }
}

template<typename T>
void DynamicArray<T>::pushBack(const T& elem) {
    ensureCapacity_();
    *storage_.at(size_) = elem;
    size_++;
}

template<typename T>
template<typename... Args>
void DynamicArray<T>::emplaceBack(Args&&... args) {
    ensureCapacity_();
    new (storage_.at(size_)) T(std::forward<Args>(args)...);
    size_++;
}

template<typename T>
void DynamicArray<T>::popBack() {
    if (size_ == 0u) {
        return;
    }
    size_--;
}

template<typename T>
void DynamicArray<T>::swapAndRemove(std::size_t index) {
    PG_ASSERT(index < size_ - 1u);
    std::swap(*storage_.at(index), *storage_.at(size_ - 1u));
    popBack();
}

template<typename T>
void DynamicArray<T>::swapAndRemove(typename DynamicArray<T>::Iterator it) {
    PG_ASSERT(it != end());
    PG_ASSERT(it != end() - 1u);
    std::swap(*it, *storage_.at(size_ - 1u));
    popBack();
}

template<typename T>
void DynamicArray<T>::swapAndRemove(typename DynamicArray<T>::ReverseIterator it) {
    PG_ASSERT(it != rend());
    PG_ASSERT(it != rbegin());
    std::swap(*it, *storage_.at(size_ - 1u));
    popBack();
}

template<typename T>
void DynamicArray<T>::clear() {
    if (size_ == 0u) {
        return;
    }
    for (auto& element : *this) {
        element.~T();
    }
    size_ = 0u;
}

template<typename T>
std::size_t DynamicArray<T>::size() const {
    return size_;
}

template<typename T>
std::size_t DynamicArray<T>::capacity() const {
    return storage_.capacity();
}

}
