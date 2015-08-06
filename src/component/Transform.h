
#pragma once

#include "math/Vector.h"

namespace pg {
namespace component {

struct Transform {
    pg::math::Vector3f position;
    pg::math::Vector3f scale;
};

}   //namespace component
}   //namespace pg

