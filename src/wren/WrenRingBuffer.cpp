#include "wren/WrenRingBuffer.h"
#include "utils/RingBuffer.h"

namespace pg {
namespace wren {

void ringBufferPushBack( WrenVM* vm ) {
    RingBuffer<float>* ring = (RingBuffer<float>*)wrenGetSlotForeign( vm, 0 );
    ring->pushBack( float(wrenGetSlotDouble( vm, 1 )) );
}

}
}
