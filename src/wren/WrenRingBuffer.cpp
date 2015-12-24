#include "wren/WrenRingBuffer.h"
#include "utils/RingBuffer.h"

namespace pg {
namespace wren {

void ringBufferPushBack( WrenVM* vm ) {
    RingBuffer<float>* ring = (RingBuffer<float>*)wrenGetArgumentForeign( vm, 0 );
    ring->pushBack( wrenGetArgumentDouble( vm, 1 ) );
}

}
}
