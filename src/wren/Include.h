
#pragma once

#include "Wrenly.h"

namespace pg {
namespace wren {
    // this namespace generates the Wren scripting language interface
    void bindMathModule();
    void bindVectorModule();
    void bindQuaternionModule();
    void bindEntityModule();
    void bindComponentModule();
}
}
