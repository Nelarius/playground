#pragma once

#include "utils/Assert.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>

namespace pg {

// In a perfect world, this class would take an allocater and use
// it for obtaining memory

/*
 * @brief Provides an uninitialized buffer for an array of T.
 * This owns the memory uniquely, which means that it is movable, but not copyable.
 *
 * This class is not type-safe, so you must call the destructor yourself
 * on objects stored there.
 */
template< typename T >
class BasicStorage {
public:
    /**
     * @brief Construct with zero capacity
     */
    BasicStorage() = default;
    /*
     * @brief Construct with a capacity to store capacity instances of T.
     */
    explicit BasicStorage(std::size_t capacity);
    BasicStorage(const BasicStorage&) = delete;
    BasicStorage& operator=(const BasicStorage&) = delete;
    BasicStorage(BasicStorage&&);
    BasicStorage& operator=(BasicStorage&&);
    ~BasicStorage();

    T* at(std::size_t index);
    const T* at(std::size_t index) const;
    T* operator[](std::size_t index);
    const T* operator[](std::size_t index) const;

    /*
     * @brief Resize to store at least newSize instances of T.
     * The actual capacity may remain the same if the requested capacity is
     * smaller than the current capacity.
     */
    void resize(std::size_t newSize);
    std::size_t capacity() const;

private:
    uint8_t* buffer_{ nullptr };
    std::size_t capacity_{ 0u };
};

template< typename T >
BasicStorage<T>::BasicStorage(std::size_t capacity) {
    if (capacity == 0u) {
        return;
    }
    capacity_ = capacity;
    buffer_ = new uint8_t[sizeof(T) * capacity];
}

template< typename T >
BasicStorage<T>::BasicStorage(BasicStorage&& other)
    : buffer_(other.buffer_),
    capacity_(other.capacity_) {
    other.buffer_ = nullptr;
    other.capacity_ = nullptr;
}

template< typename T >
BasicStorage<T>& BasicStorage<T>::operator=(BasicStorage&& rhs) {
    if (buffer_) {
        delete[] buffer_;
    }
    buffer_ = rhs.buffer_;
    capacity_ = rhs.capacity_;
    rhs.buffer_ = nullptr;
    rhs.capacity_ = nullptr;
    return *this;
}

template< typename T >
BasicStorage<T>::~BasicStorage() {
    if (buffer_) {
        delete[] buffer_;
    }
}

template< typename T >
T* BasicStorage<T>::at(std::size_t index) {
    PG_ASSERT(index < capacity_);
    return reinterpret_cast<T*>(buffer_) + index;
}

template< typename T >
const T* BasicStorage<T>::at(std::size_t index) const {
    PG_ASSERT(index < capacity_);
    return reinterpret_cast<const T*>(buffer_) + index;
}

template< typename T >
T* BasicStorage<T>::operator[](std::size_t index) {
    return at(index);
}

template< typename T >
const T* BasicStorage<T>::operator[](std::size_t index) const {
    return at(index);
}

template< typename T >
void BasicStorage<T>::resize(std::size_t newSize) {
    if (newSize == 0) {
        return;
    }
    if (capacity_ < newSize) {
        uint8_t* newBuffer = new uint8_t[sizeof(T) * newSize];
        std::memcpy(newBuffer, buffer_, sizeof(T) * capacity_);
        if (buffer_) {
            delete buffer_;
        }
        buffer_ = newBuffer;
        capacity_ = newSize;
    }
}

template< typename T >
std::size_t BasicStorage<T>::capacity() const {
    return capacity_;
}

}
