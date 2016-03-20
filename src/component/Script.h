
#pragma once

#include "Wren++.h"
#include <cstdlib>

namespace pg {
namespace component {

struct Script {
    Script(std::size_t hash, wrenpp::VM&& otherVM)
        : scriptId{ hash },
        vm{ std::move(otherVM) },
        activate{ vm.method("main", "activate", "call()") },
        deactivate{ vm.method("main", "deactivate", "call()") },
        update{ vm.method("main", "update", "call(_)") } {}

    std::size_t    scriptId;   // the hash of the script file name
    wrenpp::VM     vm;
    wrenpp::Method activate;
    wrenpp::Method deactivate;
    wrenpp::Method update;
};

}
}
