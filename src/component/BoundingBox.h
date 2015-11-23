#pragma once

#include "math/Vector.h"

namespace pg {
namespace component {

struct BoundingBox {
    math::Vector3f min;
    math::Vector3f max;
};

}
}