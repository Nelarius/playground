#pragma once

#include "ecs/Entity.h"
#include <cstdlib>

namespace pg {
namespace system {
 
struct CameraActivated {
    ecs::Entity entity;
};

struct ToggleDebugRenderer {
    bool show;
};

// emitted when a text file is updated in the file system
struct TextFileUpdated {
    std::size_t id;
};

}
}
