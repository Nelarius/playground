
#pragma once

#include "math/Vector.h"
#include "math/Quaternion.h"

namespace pg {
namespace component {

struct Transform {
    pg::math::Vec3f      position;
    pg::math::Quatf         rotation;
    pg::math::Vec3f      scale;
};

}   //namespace component
}   //namespace pg

