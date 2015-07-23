
#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "utils/Log.h"
#include <cmath>

namespace ce {

template<typename T> T Dot( const Vector2<T>& lhs, const Vector2<T>& rhs ) {
    return lhs.x*rhs.x + lhs.y*rhs.y;
}

template<typename T> T Dot( const Vector3<T>& lhs, const Vector3<T>& rhs ) {
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

template<typename T> T L2Norm( const Vector3<T>& vec ) {
    return sqrt( vec.x*vec.x + vec.y*vec.y + vec.z*vec.z );
}

template<typename T> T Length( const Vector3<T>& vec ) {
    return sqrt( vec.x*vec.x + vec.y*vec.y + vec.z*vec.z );
}

template<typename T> T SquaredLength( const Vector3<T>& vec ){
    return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
}

template<typename T> Vector3<T> Cross( const Vector3<T>& lhs, const Vector3<T>& rhs ) {
    //
}

// doesn't make sense to have this as a template, as the plane uses floats
inline Matrix4f Mirror( const Planef& plane ) {
    const Vector3f& n = plane.normal();
    float k = plane.constant();
    Matrix4f M {
        1.0f-2.0f*n.x*n.x, -2.0f*n.y*n.x, -2.0f*n.z*n.x, 2.0f*k*n.x,
        -2.0f*n.x*n.y, 1.0f-2.0f*n.y*n.y, -2.0f*n.z*n.y, 2.0f*k*n.y,
        -2.0f*n.x*n.z, -2.0f*n.y*n.z, 1.0f-2.0f*n.z*n.z, 2.0f*k*n.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return M;
}

template<typename T>
Matrix4<T> Scale( const Vector3<T>& v ) {
    return Matrix4<T> {
        v.x, 0.0f, 0.0f, 0.0f,
        0.0f, v.y, 0.0f, 0.0f,
        0.0f, 0.0f, v.z, 0.0f, 
        0.0f, 0.0f, 0.0f, 0.0f
    };
}

template<typename T>
Matrix4<T> Rotate( const Vector3<T>& axis, T angle ) {
    return Matrix4<T> {};
}

template<typename T>
Matrix4<T> Translate( const Vector3<T>& v ) {
    return Matrix4<T> {
        0.0f, 0.0f, 0.0f, v.x,
        0.0f, 0.0f, 0.0f, v.y,
        0.0f, 0.0f, 0.0f, v.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

}   // ce

