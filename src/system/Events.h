#pragma once

#include "ecs/Entity.h"
#include "math/Vector.h"
#include <cstdlib>

namespace pg {
namespace system {

struct CameraActivated {
    ecs::Entity entity;
};

struct ShowBoundingBoxes {
    bool show;
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
    math::Vec3f start;
    math::Vec3f end;
    float lifeTime; // in seconds
};

struct RenderDebugBox {
    math::Vec3f position;
    math::Vec3f scale;
    math::Vec3f color;
    float lifeTime;
};

}
}
