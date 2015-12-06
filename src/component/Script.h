
#pragma once

#include "Wrenly.h"
#include <cstdlib>

namespace pg {
namespace component {

struct Script {
    std::size_t scriptId;   // the hash of the script file name
    wrenly::Wren vm;
    wrenly::Method activate;
    wrenly::Method deactivate;
    wrenly::Method update;
};

}
}