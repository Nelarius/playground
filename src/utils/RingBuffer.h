#pragma once

#include "utils/BasicStorage.h"
#include "utils/Assert.h"
#include "utils/Log.h"
#include <cstdlib>
#include <limits>

namespace pg {

template< typename T>
class RingBuffer {
public:
    explicit RingBuffer(std::size_t capacity);
    RingBuffer() = delete;
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    RingBuffer(RingBuffer&&);
    RingBuffer& operator=(RingBuffer&&);
    ~RingBuffer();

    class Iterator {
    public:
        Iterator() = delete;
        Iterator(const Iterator&) = default;
        Iterator(Iterator&&) = default;
        Iterator& operator=(const Iterator&) = default;
        Iterator& operator=(Iterator&&) = default;
        Iterator(RingBuffer<T>* owner, std::size_t start)
            : owner_{ owner },
            curIndex_{ start }
        {}
        ~Iterator() = default;

        bool operator==(const Iterator& rhs) const {
            return owner_ == rhs.owner_ && curIndex_ == rhs.curIndex_;
        }

        bool operator!=(const Iterator& rhs) const {
            return !(this->operator==(rhs));
        }

        Iterator& operator++() {
            curIndex_++;
            return *this;
        }

        T* operator*() {
            return owner_->storage_.at(owner_->internalIndex_(curIndex_));
        }

    private:
        RingBuffer<T>*  owner_;
        std::size_t     curIndex_;
    };

    // element access
    T& at(std::size_t);
    const T& at(std::size_t) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T& operator[](std::size_t);
    const T& operator[](std::size_t) const;
    Iterator begin();
    Iterator end();

    // modifiers
    void pushBack(const T&);
    template< typename... Args >
    void emplaceBack(Args&&... args);
    void popBack();
    void popFront();
    void clear();

    // container data
    std::size_t size() const;
    std::size_t capacity() const;

private:
    // maps indices in the range (0, size_( to the internal range
    std::size_t internalIndex_(std::size_t i) const {
        ASSERT(i < size_);
        return (head_ + i) % capacity_;
    }

    std::size_t next_(std::size_t i) const {
        return (i + 1u) % capacity_;
    }

    std::size_t previous_(std::size_t i) const {
        return (i + capacity_ - 1u) % capacity_;
    }

    BasicStorage<T>     storage_;
    std::size_t         capacity_;
    std::size_t         size_;
    std::size_t         head_;
    std::size_t         tail_;
};

template< typename T >
RingBuffer<T>::RingBuffer(std::size_t capacity)
    : storage_{},
    capacity_{ capacity },
    size_{ 0u },
    head_{ 0u },
    tail_{ 0u } {
    storage_.resize(capacity);
    ASSERT(storage_.capacity() == capacity);
}

template< typename T >
RingBuffer<T>::RingBuffer(RingBuffer&& other)
    : storage_{ std::move(other.storage_) },
    capacity_{ other.capacity_ },
    size_{ other.size_ },
    head_{ other.head_ },
    tail_{ other.tail_ } {
    other.capacity_ = 0u;
    other.size_ = 0u;
    other.head_ = 0u;
    other.tail_ = 0u;
}

template< typename T >
RingBuffer<T>& RingBuffer<T>::operator=(RingBuffer&& rhs) {
    storage_ = std::move(rhs.storage_);
    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    head_ = rhs.head_;
    tail_ = rhs.tail_;
    rhs.capacity_ = 0u;
    rhs.size_ = 0u;
    rhs.head_ = 0u;
    rhs.tail_ = 0u;
    return *this;
}

template< typename T >
RingBuffer<T>::~RingBuffer() {
    clear();
}

template< typename T >
void RingBuffer<T>::clear() {
    for (auto* elem : *this) {
        elem->~T();
    }
    size_ = 0u;
    head_ = 0u;
    tail_ = 0u;
}

template< typename T >
const T& RingBuffer<T>::at(std::size_t i) const {
    ASSERT(i < size_);
    return *storage_.at((head_ + i) % size_);
}

template< typename T >
T& RingBuffer<T>::at(std::size_t i) {
    return const_cast<T&>(static_cast<const RingBuffer<T>*>(this)->at(i));
}

template< typename T >
T& RingBuffer<T>::operator[](std::size_t i) {
    return at(i);
}

template< typename T >
const T& RingBuffer<T>::operator[](std::size_t i) const {
    return at(i);
}

template< typename T >
T& RingBuffer<T>::front() {
    return *storage_.at(head_);
}

template< typename T >
const T& RingBuffer<T>::front() const {
    return *storage_.at(head_);
}

template< typename T >
T& RingBuffer<T>::back() {
    return *storage_.at(previous_(tail_));
}

template< typename T >
const T& RingBuffer<T>::back() const {
    return *storage_.at(previous_(tail_));
}

template< typename T >
typename RingBuffer<T>::Iterator RingBuffer<T>::begin() {
    return Iterator(this, 0u);
}

template< typename T >
typename RingBuffer<T>::Iterator RingBuffer<T>::end() {
    return Iterator(this, size_);
}

template< typename T >
void RingBuffer<T>::pushBack(const T& t) {
    if (size_ != 0u && head_ == tail_) {
        storage_.at(head_)->~T();
        head_ = next_(head_);
    }
    *storage_.at(tail_) = t;
    tail_ = next_(tail_);
    size_ = size_ != capacity_ ? size_ + 1u : capacity_;
}

template< typename T >
template< typename... Args>
void RingBuffer<T>::emplaceBack(Args&&... args) {
    if (size_ != 0u && head_ == tail_) {
        storage_.at(head_)->~T();
        head_ = next_(head_);
    }
    new (storage_.at(tail_)) T(std::forward<Args>(args)...);
    tail_ = next_(tail_);
    size_ = size_ != capacity_ ? size_ + 1u : capacity_;
}

template< typename T >
void RingBuffer<T>::popBack() {
    ASSERT(size_ != 0u);
    tail_ = previous_(tail_);
    storage_.at(tail_)->~T();
    size_--;
}

template< typename T >
void RingBuffer<T>::popFront() {
    ASSERT(size_ != 0u);
    storage_.at(head_)->~T();
    head_ = next_(head_);
    size_--;
}

template< typename T >
std::size_t RingBuffer<T>::size() const {
    return size_;
}

template< typename T >
std::size_t RingBuffer<T>::capacity() const {
    return capacity_;
}

}
