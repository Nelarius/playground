
#pragma once

#include "math/Vector.h"
#include "math/Quaternion.h"

namespace pg {
namespace component {

struct Transform {
    pg::math::Vector3f      position;
    pg::math::Quatf   orientation;
    pg::math::Vector3f      scale;
};

}   //namespace component
}   //namespace pg

