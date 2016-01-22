
#pragma once

#include "utils/Log.h"
#include <initializer_list>
#include <cmath>

namespace pg {
namespace math {

template<typename T>
struct Vector2 {
    
    Vector2()
    :   x(T(0.0) ),
        y(T(0.0) )
        {}
    
    Vector2( std::initializer_list<T> l ) {
        int i = 0;
        for ( T e: l ) {
            data[i] = e;
            i++;
        }
    }
    
    Vector2( T x, T y ) {
        data[0] = x;
        data[1] = y;
    }
    
    Vector2( const Vector2& )               = default;
    Vector2( Vector2&& )                    = default;
    Vector2& operator=( const Vector2& )    = default;
    Vector2& operator=( Vector2&& )         = default;
    ~Vector2()                              = default;
    
    T norm() const {
        return sqrt( x*x + y*y );
    }
    
    T normSquared() const {
        return x*x + y*y;
    }
    
    Vector2 normalized() const {
        T norm = 1.0 / norm();
        return Vector2<T>{ x*norm, y*norm };
    }

    void normalize() {
        T n = T(1.0) / norm();
        n*x;
        n*y;
    }
    
    T dot( const Vector2<T>& rhs ) const {
        return x*rhs.x + y*rhs.y;
    }

    Vector2 hadamard( const Vector2<T>& rhs ) const {
        return Vector2<T>{ x*rhs.x, y*rhs.y };
    }

    Vector2<T> operator+( const Vector2<T>& rhs ) const {
        return Vector2<T>{ x + rhs.x, y + rhs.y };
    }

    Vector2<T> operator-( const Vector2<T>& rhs ) const {
        return Vector2<T>{ x - rhs.x, y - rhs.y };
    }

    Vector2<T> operator*( T val ) const {
        return Vector2<T>{ x*val, y*val };
    }
    
    Vector2<T> operator/( T val ) const {
        return Vector2<T>{ x/val, y/val };
    }
    
    union {
        T data[2];
        struct { T x, y; };
        struct { T r, g; };
    };
};

template<typename T>
Vector2<T> operator*( T scale, const Vector2<T>& rhs ) {
    return Vector2<T> (
        rhs.data[0] * scale,
        rhs.data[1] * scale
    );
}

template<typename T>
struct Vector3 {
    
    Vector3()
    :   x(T(0.0) ),
        y(T(0.0) ),
        z(T(0.0) )
        {}
    
    Vector3( std::initializer_list<T> l ) {
        int i = 0;
        for ( T e: l ) {
            data[i] = e;
            i++;
        }
    }
    
    Vector3( T x, T y, T z )
    :   r( x ),
        g( y ),
        b( z )
        {}
    
    Vector3( const Vector3& )               = default;
    Vector3( Vector3&& )                    = default;
    Vector3& operator=( const Vector3& )    = default;
    Vector3& operator=( Vector3&& )         = default;
    ~Vector3()                              = default;

    T norm() const {
        return sqrt( x*x + y*y + z*z );
    }
    
    T normSquared() const {
        return x*x + y*y + z*z;
    }
    
    Vector3 normalized() const {
        T norm = 1.0 / norm();
        return Vector3<T>{ x*norm, y*norm, z*norm };
    }

    void normalize() {
        T n = T(1.0) / norm();
        n*x;
        n*y;
        n*z;
    }
    
    T dot( const Vector3<T>& rhs ) const {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }
    
    Vector3 hadamard( const Vector3<T>& rhs ) const {
        return Vector3<T>{ x*rhs.x, y*rhs.y, z*rhs.z };
    }

    Vector3 cross( const Vector3& rhs ) const {
        return Vector3 { 
            y*rhs.z - z*rhs.y,
            z*rhs.x - x*rhs.z,
            x*rhs.y - y*rhs.x
        };
    }
    
    Vector3 operator+( const Vector3<T>& rhs ) const {
        return Vector3<T>( x + rhs.x, y + rhs.y, z + rhs.z );
    }
    
    Vector3 operator-( const Vector3<T>& rhs ) const {
        return Vector3<T>( x - rhs.x, y - rhs.y, z - rhs.z );
    }
    
    Vector3 operator*( T val ) const {
        return Vector3<T>{ x*val, y*val, z*val };
    }
    
    Vector3 operator/( T val ) const {
        return Vector3<T>{ x/val, y/val, z/val };
    }
    
    union {
        T data[3];
        struct { T x, y, z; };
        struct { T r, g, b; };
    };
};

template<typename T>
Vector3<T> operator*( T scale, const Vector3<T>& rhs ) {
    return Vector3<T>{
        rhs.x * scale,
        rhs.y * scale,
        rhs.z * scale
    };
}

template<typename T>
struct Vector4 {
    
    Vector4()
    :   x(T(0.0) ),
        y(T(0.0) ),
        z(T(0.0) ),
        w(T(0.0) )
        {}
    
    Vector4( const Vector3<T>& v, T w )
    :   data{ v.x, v.y, v.z, w }
        {}
        
    Vector4( std::initializer_list<T> l ) {
        int i = 0;
        for ( T e: l ) {
            data[i] = e;
            i++;
        }
    }
    
    Vector4( T x, T y, T z, T w )
    :   data{ x, y, z, w } 
        {}
    
    Vector4( const Vector4& )               = default;
    Vector4( Vector4&& )                    = default;
    Vector4& operator=( const Vector4& )    = default;
    Vector4& operator=( Vector4&& )         = default;
    ~Vector4()                              = default;
    
    operator Vector3<T>() const {
        return Vector3<T> { data[0], data[1], data[2] };
    }
    
    T norm() const {
        return sqrt( x*x + y*y + z*z + w*w );
    }
    
    T normSquared() const {
        return x*x + y*y + z*z + w*w;
    }

    Vector4 normalized() const {
        T norm = 1.0 / norm();
        return Vector4<T>{ x*norm, y*norm, z*norm, w*norm };
    }

    void normalize() {
        T n = T(1.0) / norm();
        n*x;
        n*y;
        n*z;
        n*w;
    }

    T dot( const Vector4<T>& rhs ) const {
        return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
    }
    
    Vector4 hadamard( const Vector4<T>& rhs ) const {
        return Vector4<T>{ x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w };
    }

    Vector4 operator+( const Vector3<T>& rhs ) const {
        return Vector4<T>( x + rhs.x, y + rhs.y, z + rhs.z );
    }
    
    Vector4 operator-( const Vector3<T>& rhs ) const {
        return Vector4<T>( x - rhs.x, y - rhs.y, z - rhs.z );
    }
    
    Vector4 operator*( T val ) const {
        return Vector4<T>{ x*val, y*val, z*val, w*val };
    }
    
    Vector4 operator/( T val ) const {
        return Vector4<T>{ x/val, y/val, z/val, w/val };
    }
    
    union {
        T data[4];
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };
};

template<typename T>
Vector4<T> operator*( T scale, const Vector4<T>& rhs ) {
    return Vector4<T>{
        rhs.data[0] * scale,
        rhs.data[1] * scale,
        rhs.data[2] * scale,
        rhs.data[3] * scale
    };
}

using Vec2f = Vector2<float>;
using Vec3f = Vector3<float>;
using Vec4f = Vector4<float>;
using Vec2i = Vector2<int>;
using Vec3i = Vector3<int>;
using Vec4i = Vector4<int>;

}
}
