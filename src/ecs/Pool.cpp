#include "ecs/Pool.h"

using ce::ecs::BasePool;

BasePool::BasePool( std::size_t elementSize, std::size_t chunkSize )
:   blocks_(),
    elementSize_( elementSize ),
    chunkSize_( chunkSize ),
    capacity_( 0u ) {
    this->reserve( chunkSize_ );
}

BasePool::~BasePool() {
    for ( auto block: blocks_ ) {
        delete [] block;
    }
}

std::size_t BasePool::capacity() const {
    return capacity_;
}

std::size_t BasePool::chunks() const {
    return blocks_.size();
}

void BasePool::reserve( std::size_t n ) {
    while ( capacity_ <= n ) {
        char* chunk = new char[ elementSize_ * chunkSize_ ];
        blocks_.push_back( chunk );
        capacity_ += chunkSize_;
    }
}

void* BasePool::at( std::size_t i ) {
    if ( i >= capacity_ ) {
        reserve( i );
    }
    return blocks_[ i / chunkSize_ ] + ( i % chunkSize_ ) * elementSize_;
}

const void* BasePool::at( std::size_t i ) const {
    if ( i >= capacity_ ) {
        reserve( i );
    }
    return blocks_[ i / chunkSize_ ] + ( i % chunkSize_ ) * elementSize_;
}
