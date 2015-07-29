#include "utils/Pool.h"

using ce::BasePool;

BasePool::BasePool( std::size_t elementSize, std::size_t chunkSize )
:   blocks_(),
    ElementSize_( elementSize ),
    ChunkSize_( chunkSize ),
    capacity_( 0u ) {
    this->reserve( ChunkSize_ );
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
        char* chunk = new char[ ElementSize_ * ChunkSize_ ];
        blocks_.push_back( chunk );
        capacity_ += ChunkSize_;
    }
}

void* BasePool::at( std::size_t i ) {
    if ( i >= capacity_ ) {
        reserve( i );
    }
    return blocks_[ i / ChunkSize_ ] + ( i % ChunkSize_ ) * ElementSize_;
}

