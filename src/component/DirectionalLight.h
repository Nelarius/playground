#pragma once 

#include "math/Vector.h"

namespace pg {
namespace component {

struct DirectionalLight {
    math::Vec3f color;
    math::Vec3f direction;
    float ambientCoefficient;
};

}
}
