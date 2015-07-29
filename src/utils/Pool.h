
#pragma once

#include "utils/Assert.h"
#include <cstddef>  // for std::size_t
#include <vector>
#include <set>

namespace ce {

/**
 * @class BasePool
 * @author Muszynski Johann M
 * @date 06/10/15
 * @file Pool.h
 * @brief A pool of mostly contiguous memory, organized into discrete chunks.
 * This is a "stupid pool", as it doesn't do any sort of memory management.
 * Pointers into the pool will only be invalidated when the pool is destroyed.
 * 
 * Lookups are O(1).
 * Appends are amortized O(1).
 */
class BasePool {
    public:
        BasePool() = delete;
        /**
         * @brief Construct the pool with the element and chunk size.
         * @param elementSize The size of one element, in bytes.
         * @param chunkSize The number of elements that you want stored in one chunk.
         */
        explicit BasePool( std::size_t elementSize, std::size_t chunkSize = 128 );
        virtual ~BasePool();
        
        BasePool( const BasePool& )             = delete;
        BasePool& operator=( const BasePool& )  = delete;
        BasePool( BasePool&& )                  = delete;
        BasePool& operator=( BasePool&& )       = delete;

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
        void reserve( std::size_t n );
        /**
         * @brief Get the pointer to the n:th element in the chunk.
         * @param The n:th element, as if the chunk of memory were an array.
         * When n is larger than the current capacity, the memory pool will resize itself,
         * instead of causing a segfault or shutting the program down. The reason for this is
         * the design of the main user of this class: EntityManager.
         * 
         * EntityManager does not create a memory pool for a specific type until that type of
         * component is assigned for the first time. When we do the assignment, it is easy to
         * simply create the pool (done in EntityManager::accommodateComponent_), and then immediately
         * use placement new (using this method) to create the component, thus not having to worry
         * separately about resizing to the correct size.
         */
        virtual void* at( std::size_t n );
        /**
         * @brief Destroy element at position i.
         */
        virtual void destroy( std::size_t i ) = 0;

    protected:
        std::vector<char*>  blocks_{};
        const std::size_t   ElementSize_;
        const std::size_t   ChunkSize_; // the number of elements in a chunk
        std::size_t         capacity_{ 0u };    // corresponds to the size
};

/**
 * @class Pool
 * @author Muszynski Johann M
 * @date 06/14/15
 * @file Pool.h
 * @brief 
 */
template<typename T>
class Pool: public BasePool {
    public:
        /**
         * @brief Create the Pool with the chunk size.
         * @param chunkSize The number of elements one block of memory will hold.
         */
        Pool( std::size_t chunkSize )
        :   BasePool( sizeof(T), chunkSize )
            {}
        virtual ~Pool() = default;
        
        void destroy( std::size_t n ) override {
            ASSERT( n < capacity_, "Pool::destroy> invalid positional argument." );
            T* ptr = static_cast<T*>( this->at( n ) );
            ptr->~T();
        }
};

}   // namespace ce
