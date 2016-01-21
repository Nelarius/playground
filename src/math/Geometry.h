#pragma once

#include "math/Vector.h"

namespace pg {
namespace math {

struct Ray {
    Vector3f origin;
    Vector3f direction;
    float t;
};

struct Line {
    Vector3f origin;
    Vector3f end;
};

// Axis-aligned box
struct AABox {
    Vector3f min;
    Vector3f max;
};

}
}
