
#pragma once

#include "Wrenly.h"

namespace pg {
namespace component {

struct WrenScript {
    wrenly::Wren vm;
    wrenly::Method activate;
    wrenly::Method deactivate;
    wrenly::Method update;
};

}
}