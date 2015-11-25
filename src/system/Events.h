#pragma once

#include "ecs/Entity.h"

namespace pg {
namespace system {
 
struct CameraActivated {
    ecs::Entity entity;
};

struct ToggleDebugRenderer {
    bool show;
};

}
}