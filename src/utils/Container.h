
#pragma once 

#include "utils/MemoryArena.h"
#include "utils/Assert.h"
#include <cstdlib>
#include <cstdint>

namespace pg {

/**
 * @class Container
 * @author Muszynski Johann M
 * @date 29/07/15
 * @file Container.h
 * @brief Emplace data structures that don't have move or copy constructors.
 * This container can only be grown.
 * This data structure was needed due to the need to store all the unmovable data structures in contiguous blocks.
 *
 * Container does not resize like std::vector does. It creates a new, fixed size block of memory
 * every time a resize is needed. Thus, not all components are contiguous.
 */
template<typename T, std::size_t N = 64>
class Container {
public:
    Container() = default;
    Container(const Container&) = default;
    Container(Container&&) = delete;
    Container& operator=(const Container&) = delete;
    Container& operator=(Container&&) = delete;
    ~Container() {
        clear();
    }
    /**
     * @brief Emplace at the back.
     * @return The index at which the element is located.
     * Pass the aggregate initialization argument to this mehtod.
     */
    template< typename... Args >
    std::size_t emplace(Args&&... args) {
        new (pool_.newCapacity(position_)) T{ std::forward<Args>(args)... };
        return position_++;
    }

    const T& at(std::size_t i) const {
        ASSERT(i < position_);
        return *(static_cast<const T*>(pool_.at(i)));
    }

    T& at(std::size_t i) {
        return const_cast<T&>(static_cast<const Container&>(*this)[i]);
    }

    const T& operator[](std::size_t i) const {
        return at(i);
    }

    T& operator[](std::size_t i) {
        return at(i);
    }

    std::size_t size() const {
        return position_;
    }

    void clear() {
        for (std::size_t i = 0u; i < position_; i++) {
            pool_.destroy(i);
        }
        position_ = 0u;
    }

private:
    MemoryArena<T>  pool_{ N };
    std::size_t     position_{ 0u };
};


}   // ce

