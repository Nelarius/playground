
#pragma once

#include "Wrenly.h"

namespace pg {
namespace wren {
    // this namespace generates the Wren scripting language interface
    void GenerateMathModule( wrenly::Wren& );
    void GenerateVectorModule( wrenly::Wren& );
}
}
