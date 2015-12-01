#pragma once

#include "Wrenly.h"
extern "C" {
    #include <wren.h>
}

namespace pg {
namespace wren {

void conjugate( WrenVM* vm );
void inverse( WrenVM* vm );
void multiply( WrenVM* vm );
void axis( WrenVM* vm );

}
}