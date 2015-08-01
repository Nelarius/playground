
#pragma once 

#include "utils/Pool.h"
#include "utils/Assert.h"
#include <cstdlib>
#include <cstdint>

namespace ce {

/**
 * @class Bundle
 * @author Muszynski Johann M
 * @date 29/07/15
 * @file Bundle.h
 * @brief Emplace data structures that don't have move or copy constructors.
 * This data structure was needed due to the need to store all the unmovable data structures
 * in contiguous blocks. The idea is that the client keeps track of the position of the 
 * emplaced object, e.g. frequently in std::unordered_map.
 * 
 * Bundle does not resize like std::vector does. It creates a new, fixed size block of memory
 * every time a resize is needed. Thus, not all components are contiguous.
 * 
 * By default, 64 elements are stored in one block.
 */
template<typename T, std::size_t N = 64>
class Bundle {
    public:
        Bundle()                            = default;
        Bundle( const Bundle& )             = default;
        Bundle( Bundle&& )                  = delete;
        Bundle& operator=( const Bundle& )  = delete;
        Bundle& operator=( Bundle&& )       = delete;
        ~Bundle() {
            for ( std::size_t i = 0u; i < position_; i++ ) {
                pool_.destroy( i );
            }
        }
        /**
         * @brief Emplace at the back.
         * @return The index at which the element is located.
         * Pass the aggregate initialization argument to this mehtod.
         */
        template<typename... Args>
        std::size_t emplace( Args&&... args ) {
            new ( pool_.at( position_ ) ) T{ std::forward<Args>( args )... };
            return position_++;
        }
        T& at( std::size_t i ) {
            ASSERT( i < position_, "Bundle::at> index out of bound" );
            return *(static_cast<T*>( pool_.at(i) ));
        }
        T& operator[]( std::size_t i ) {
            ASSERT( i < position_, "Bundle::operator[]> index out of bound" );
            return *(static_cast<T*>( pool_.at(i) ));
        }
        std::size_t size() const {
            return position_;
        }
        
    private:
        Pool<T>     pool_{ N };
        std::size_t position_{ 0u };
};


}   // ce

