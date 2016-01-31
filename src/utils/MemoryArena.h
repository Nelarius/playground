
#pragma once

#include "utils/Assert.h"
#include <cstddef>  // for std::size_t
#include <vector>
#include <set>

namespace pg {

/**
 * @class BaseArena
 * @author Muszynski Johann M
 * @date 06/10/15
 * @file MemoryArena.h
 * @brief A pool of mostly contiguous memory, organized into discrete chunks.
 * This is a "stupid pool", as it doesn't do any sort of memory management.
 * Pointers into the pool will only be invalidated when the pool is destroyed.
 *
 * Lookups are O(1).
 * Appends are amortized O(1).
 */
class BaseArena {
public:
    BaseArena() = delete;
    /**
     * @brief Construct the pool with the element and chunk size.
     * @param elementSize The size of one element, in bytes.
     * @param chunkSize The number of elements that you want stored in one chunk.
     */
    explicit BaseArena(std::size_t elementSize, std::size_t chunkSize = 128);
    virtual ~BaseArena();

    BaseArena(const BaseArena&) = delete;
    BaseArena& operator=(const BaseArena&) = delete;
    BaseArena(BaseArena&&) = delete;
    BaseArena& operator=(BaseArena&&) = delete;

    /**
     * @brief Get the total capacity (the number of elements) of the contained memory pool.
     */
    std::size_t capacity() const;
    /**
     * @brief Get the number of memory chunks contained in the pool.
     */
    std::size_t chunks() const;
    /**
     * @brief Reserve space for at least n elements.
     * @param n The number of elements
     * This method may allocate more memory than needed for n elements,
     * as it allocates memory one chunk size at a time.
     */
    void reserve(std::size_t n);

    virtual const void* at(std::size_t n) const;
    virtual void* at(std::size_t n);

    /**
     * @brief Get the pointer to the n:th element in the chunk.
     * @param The n:th element, as if the chunk of memory were an array.
     * When n is larger than the current capacity, the memory pool will resize itself.
     */
    void* newCapacity(std::size_t n);

    /**
     * @brief Destroy element at position i.
     */
    virtual void destroy(std::size_t i) = 0;

protected:
    std::vector<char*>  blocks_{};
    const std::size_t   ElementSize_;
    const std::size_t   ChunkSize_; // the number of elements in a chunk
    std::size_t         capacity_{ 0u };    // corresponds to the size
};

/**
 * @class MemoryArena
 * @author Muszynski Johann M
 * @date 06/14/15
 * @file MemoryArena.h
 * @brief
 */
template<typename T>
class MemoryArena : public BaseArena {
public:
    /**
     * @brief Create the MemoryArena with the chunk size.
     * @param chunkSize The number of elements one block of memory will hold.
     */
    MemoryArena(std::size_t chunkSize)
        : BaseArena(sizeof(T), chunkSize)
    {}
    virtual ~MemoryArena() = default;

    void destroy(std::size_t n) override {
        PG_ASSERT(n < capacity_);
        T* ptr = static_cast<T*>(this->at(n));
        ptr->~T();
    }
};

}   // namespace pg
