#pragma once

#include "math/Vector.h"

namespace pg {
namespace math {

struct Ray {
    Vec3f origin;
    Vec3f direction;
    float t;
};

struct Line {
    Vec3f origin;
    Vec3f end;
};

// Axis-aligned box
struct AABox {
    Vec3f min;
    Vec3f max;
};

}
}
