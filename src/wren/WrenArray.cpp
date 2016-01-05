#include "wren/WrenArray.h"
#include "utils/Log.h"
#include <vector>

namespace pg {
namespace wren {

void arrayPushBack( WrenVM* vm ) {
    std::vector<float>* array = (std::vector<float>*)wrenGetSlotForeign( vm, 0 );
    array->push_back( float(wrenGetSlotDouble( vm, 1 )) );
}

void arrayAt( WrenVM* vm ) {
    const std::vector<float>* array = (const std::vector<float>*)wrenGetSlotForeign( vm, 0 );
    std::size_t i = (std::size_t)wrenGetSlotDouble( vm, 1 );
    if ( i >= array->size() ) {
        LOG_ERROR << "Script error> NumberArray: index out of bounds. Size: " << array->size() << ", index: " << i;
        return;
    }
    wrenSetSlotDouble( vm, 0, double( array->at(i) ) );
}

}
}
