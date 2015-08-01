
#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Plane.h"
#include "utils/Log.h"
#include <cmath>

namespace ce {
namespace math {

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

/**
 * @brief 
 * @param near near plane, as a distance from the eye
 * @param far far plane, as a distance from the eye
 * @param fov The vertical field of view
 * @param width
 * @param height
 * @return 
 */
template<typename T>
Matrix4<T> Perspective( T near, T far, T fov, T width, T height ) {
    return Matrix4<T> {
        near / (width/2), 0, 0, 0,
        0, near / (height/2), 0, 0,
        0, 0, -(far + near) / (far - near), 2*(far * near) / (far - near),
        0, 0, -1, 0
    };
}

template<typename T>
Matrix4<T> AsymmetricPerspective( T near, T far, T fov, T aspect ) {
    return Matrix4<T>{};
}

template<typename T>
Matrix4<T> Orthographic() {
    return Matrix4<T>{
        
    };
}

template<typename T>
Matrix4<T> Model( const Vector3<T>& s, const Vector3<T>& t) {
    // the composite model matrix is C = TRS
    // where T is the translation matrix
    // R is the rotation matrix 
    // S is the scale matrix
    return Translate( t ) * Scale( s );
}

template<typename T>
Matrix4<T> Camera() {
    return Matrix4<T> {};
}

}   // math
}   // ce

