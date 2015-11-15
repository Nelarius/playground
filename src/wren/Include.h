
#pragma once

#include "Wrenly.h"

namespace pg {
namespace wren {
    // this namespace generates the Wren scripting language interface
    void BindMathModule( wrenly::Wren& );
    void BindVectorModule( wrenly::Wren& );
}
}
