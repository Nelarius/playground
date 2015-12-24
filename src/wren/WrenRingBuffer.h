#pragma once

extern "C" {
    #include <wren.h>
}

namespace pg {
namespace wren {

void ringBufferPushBack( WrenVM* );

}
}
