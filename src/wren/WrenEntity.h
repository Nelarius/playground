#pragma once

#include "component/Include.h"
#include "math/Vector.h"
extern "C" {
    #include <wren.h>
}

namespace pg {
namespace wren {

void set( WrenVM* );
void entityIndex( WrenVM* );
void entityVersion( WrenVM* );
void setTransform( WrenVM* );

}
}
