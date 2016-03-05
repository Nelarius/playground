#pragma once

#include "math/Vector.h"

namespace pg {
namespace math {

template<typename T>
struct Ray {
    Vector3<T> origin;
    Vector3<T> direction;
    T t;
};

template<typename T>
struct Line {
    Vector3<T> origin;
    Vector3<T> end;
};

template<typename T>
struct AABox {
    Vector3<T> min;
    Vector3<T> max;

    inline Vector3<T> center() const {
        return T(0.5) * (min + max);
    }
};

template<typename T>
struct Sphere {
    Vector3<T> center;
    T radius;
};

using Rayf = Ray<float>;
using Linef = Line<float>;
using AABoxf = AABox<float>;
using Spheref = Sphere<float>;

}
}
