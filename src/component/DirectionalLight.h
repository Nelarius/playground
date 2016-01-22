
#pragma once 

#include "math/Vector.h"

namespace pg {
namespace component {

struct DirectionalLight {
    math::Vec3f intensity;
    float ambientCoefficient;
};

}
}