#pragma once

#include "math/Matrix.h"
#include "math/Vector.h"
#include <limits>
#include <cmath>

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
struct Frustum {
    Frustum(T fov, T aspectRatio, T n, T f)
        : width{ aspectRatio * T(2.0) * n * tan(fov / T(2.0)) },
        height{ T(2.0) * n * tan(fov / T(2.0)) },
        near{ n },
        far{ f } {}
    T width{}, height{};
    T near{}, far{};
};

template<typename T>
Ray<T> generateCameraRay(const Vector3<T>& eye, const Quaternion<T>& orientation, const Frustum<T>& frustum, T x, T y) {
    Vector3<T> eyeToPixel{
        x * T(0.5) * frustum.width,
        y * T(0.5) * frustum.height,
        -frustum.near
    };   // untranslated pixel coordinates
    Vector4<T> temp = Matrix4<T>::rotation(orientation) * Vector4<T>(eyeToPixel, T(1.0));
    eyeToPixel = Vector3<T>{ temp.x, temp.y, temp.z };
    eyeToPixel.normalize();
    return Ray<T>{ eye, eyeToPixel, std::numeric_limits<T>::max() };
}

/*
 * The plane normal is calculated from the input points in a counter clockwise sense,
 * according to the right hand rule.
 **/
template<typename T>
class Plane {
public:
    Plane() = delete;
    Plane(const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2)
        : edge1_{ p1 - p0 },
        edge2_{ p2 - p0 },
        point_{ p0 },
        normal_{ edge1_.cross(edge2_).normalized() } {}
    ~Plane() = default;

    const Vector3<T> point() const {
        return point_;
    }

    const Vector3<T> normal() const {
        return normal_;
    }

private:
    Vector3<T> edge1_;
    Vector3<T> edge2_;
    Vector3<T> point_;
    Vector3<T> normal_;
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
using Frustumf = Frustum<float>;
using Planef = Plane<float>;
using AABoxf = AABox<float>;
using Spheref = Sphere<float>;

}
}
