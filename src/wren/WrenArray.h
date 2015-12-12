#pragma once

extern "C" {
    #include <wren.h>
}

namespace pg {
namespace wren {

void arrayPushBack( WrenVM* );
void arrayAt( WrenVM* );

}
}
