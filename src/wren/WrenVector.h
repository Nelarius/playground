#pragma once

#include "math/Vector.h"
extern "C" {
    #include <wren.h>
}

namespace pg {
namespace wren {

void cross3f( WrenVM* vm );
void plus3f( WrenVM* vm );
void minus3f( WrenVM* vm );
void hadamard3f( WrenVM* vm );

}
}