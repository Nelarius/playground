
#pragma once

#include "math/Vector.h"

namespace pg {
namespace component {

struct PointLight {
    math::Vec3f intensity;
    float attenuation;
    float ambientCoefficient;
};

}
}