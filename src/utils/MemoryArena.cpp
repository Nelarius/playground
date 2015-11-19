#include "utils/MemoryArena.h"

namespace pg {

BaseArena::BaseArena( std::size_t elementSize, std::size_t chunkSize )
:   blocks_(),
    ElementSize_( elementSize ),
    ChunkSize_( chunkSize ),
    capacity_( 0u ) {
    this->reserve( ChunkSize_ );
}

BaseArena::~BaseArena() {
    for ( auto block: blocks_ ) {
        delete [] block;
    }
}

std::size_t BaseArena::capacity() const {
    return capacity_;
}

std::size_t BaseArena::chunks() const {
    return blocks_.size();
}

void BaseArena::reserve( std::size_t n ) {
    while ( capacity_ <= n ) {
        char* chunk = new char[ ElementSize_ * ChunkSize_ ];
        blocks_.push_back( chunk );
        capacity_ += ChunkSize_;
    }
}

const void* BaseArena::at( std::size_t i ) const {
    ASSERT( i < capacity_ );
    return blocks_[ i / ChunkSize_ ] + ( i % ChunkSize_ ) * ElementSize_;
}

void* BaseArena::at( std::size_t i ) {
    return const_cast<void*>( static_cast<const BaseArena&>(*this).at(i) );
}

void* BaseArena::newCapacity( std::size_t i ) {
    if ( i >= capacity_ ) {
        reserve( i );
    }
    return at( i );
}


}