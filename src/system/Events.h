#pragma once

#include "ecs/Entity.h"
#include "math/Vector.h"
#include <cstdlib>

namespace pg {
namespace system {

struct CameraActivated {
    ecs::Entity entity;
};

struct ShowDebugBoxes {
    bool show;
};

struct ShowDebugLines {
    bool show;
};

// emitted when a text file is updated in the file system
struct TextFileUpdated {
    std::size_t id;
};

struct RenderDebugLine {
    math::Vector3f start;
    math::Vector3f end;
    float lifeTime; // in seconds
};

}
}
