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

    inline Vec3f center() const {
        return 0.5f * (min + max);
    }
};

struct Sphere {
    Vec3f center;
    float radius;
};

}
}
