#pragma once

#include "ecs/Entity.h"

namespace pg {
namespace system {
 
struct PerspectiveCameraAdded {
    ecs::Entity entity;
};

struct CameraActivated {
    ecs::Entity entity;
};

}
}