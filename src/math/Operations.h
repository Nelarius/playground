
#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "utils/Log.h"
#include <cmath>

namespace pg {
namespace math {

template<typename T>
Matrix4<T> Scale( const Vector3<T>& v ) {
    return Matrix4<T> {
        v.x,    0.0f,   0.0f,   0.0f,
        0.0f,   v.y,    0.0f,   0.0f,
        0.0f,   0.0f,   v.z,    0.0f, 
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

/**
 * @brief Get the rotation matrix for the rotation between two axes.
 * @param s
 * @param t
 * @return 
 */
template<typename T>
Matrix4<T> Rotate( const Vector3<T>& s, const Vector3<T> t ) {
    s = s.normalize();
    t = t.normalize();
    Vector3<T> v = s.cross( t );
    T e = s.dot( t );
    T h = 1.0 / ( 1.0 + e );
    return Matrix4<T> {
        e + h*v.x*v.x, h*v.x*v.y - v.z, h*v.x*v.z + v.y, 0.0,
        h*v.x*v.y + v.z, e + h*v.y*v.y, h*v.y*v.z - v.x, 0.0,
        h*v.x*v.z - v.y, h*v.y*v.z + v.x, e + h*v.z*v.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
}

template<typename T>
Matrix4<T> Translate( const Vector3<T>& v ) {
    return Matrix4<T> {
        1.0f, 0.0f, 0.0f, v.x,
        0.0f, 1.0f, 0.0f, v.y,
        0.0f, 0.0f, 1.0f, v.z,
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
Matrix4<T> Model( const Vector3<T>& t, const Quaternion<T>& r, const Vector3<T>& s ) {
    // the composite model matrix is C = TRS
    // where T is the translation matrix
    // R is the rotation matrix 
    // S is the scale matrix
    return Translate( t ) * r.asMatrix() * Scale( s );
}

}   // math
}   // ce

