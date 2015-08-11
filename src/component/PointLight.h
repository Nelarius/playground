
#pragma once

#include "math/Vector.h"

namespace pg {
namespace component {

struct PointLight {
    math::Vector3f intensity;
    float attenutation;
    float ambientCoefficient;
};
    
}
}