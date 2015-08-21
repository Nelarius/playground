
#pragma once 

#include "math/Vector.h"

namespace pg {
namespace component {

struct DirectionalLight {
    math::Vector3f intensity;
    float ambientCoefficient;
};

}
}