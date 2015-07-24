
#pragma once

#include "utils/Log.h"
#include <initializer_list>
#include <cmath>

namespace ce {
namespace math {

template<typename T, typename Derived, int N>
struct BaseVector {
    BaseVector()                                = default;
    BaseVector( const BaseVector& )             = default;
    BaseVector( BaseVector&& )                  = default;
    BaseVector& operator=( const BaseVector& )  = default;
    BaseVector& operator=( BaseVector&& )       = default;
    virtual ~BaseVector() = default;
    
    T dot( const Derived& rhs ) const {
        T sum = 0;
        for ( int i = 0; i < N; i++ ) {
            sum += static_cast<Derived const*>(this)->data[i] * rhs.data[i];
        }
        return sum;
    }
    
    Derived normalize() const {
        Derived res( *static_cast<Derived const*>(this) );
        T factor = 1 / this->l2norm();
        return res * factor;
    }
    
    T l2norm() const {
        T sum = 0;
        for ( int i = 0; i < N; i++ ) {
            T element = static_cast<Derived const*>( this )->data[i];
            sum += element * element;
        }
        return sqrt(sum);
    }
    
    T length() const {
        return l2norm();
    }
    
    T squaredLength() const {
        T sum = 0;
        for ( int i = 0; i < N; i++ ) {
            T element = static_cast<Derived const*>( this )->data[i];
            sum += element * element;
        }
        return sum;
    }
    
    Derived operator+( const Derived& rhs ) const {
        Derived res{};
        for ( int i = 0; i < N; i++ ) {
            res.data[i] = static_cast<Derived const*>(this)->data[i] + rhs.data[i];
        }
        return res;
    }
    
    Derived operator-( const Derived& rhs ) const {
        Derived res{};
        for ( int i = 0; i < N; i++ ) {
            res.data[i] = static_cast<Derived const*>(this)->data[i] - rhs.data[i];
        }
        return res;
    }
    
    Derived operator*( T scale ) const {
        Derived res{};
        for ( int i = 0; i < N; i++ ) {
            res.data[i] = static_cast<Derived const*>(this)->data[i] * scale;
        }
        return res;
    }
    
    Derived operator/( T scale ) const {
        Derived res{};
        for ( int i = 0; i < N; i++ ) {
            res.data[i] = static_cast<Derived const*>(this)->data[i] * scale;
        }
        return res;
    }
    
    T data[N];
    int dim{ N };
};

template<typename T>
struct Vector2: public BaseVector<T, Vector2<T>, 2> {
    Vector2( std::initializer_list<T> l ) {
        int i = 0;
        for ( float e: l ) {
            data[i] = e;
            i++;
        }
    }
    Vector2()                               = default;
    Vector2( const Vector2& )               = default;
    Vector2( Vector2&& )                    = default;
    Vector2& operator=( const Vector2& )    = default;
    Vector2& operator=( Vector2&& )         = default;
    virtual ~Vector2()                      = default;
    
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
Vector2<T> operator/( T divisor, const Vector2<T>& rhs ) {
    T scale = 1 / divisor;
    return Vector2<T>(
        rhs.data[0] * scale,
        rhs.data[1] * scale
    );
}

template<typename T>
struct Vector3: public BaseVector<T, Vector3<T>, 3> {
    Vector3( std::initializer_list<T> l ) {
        int i = 0;
        for ( float e: l ) {
            data[i] = e;
            i++;
        }
    }
    Vector3( T x, T y, T z ) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }
    Vector3()                               = default;
    Vector3( const Vector3& )               = default;
    Vector3( Vector3&& )                    = default;
    Vector3& operator=( const Vector3& )    = default;
    Vector3& operator=( Vector3&& )         = default;
    virtual ~Vector3()                      = default;
    
    Vector3 cross( const Vector3& rhs ) const {
        return Vector3 { 
            y*rhs.z - z*rhs.y,
            z*rhs.x - x*rhs.z,
            x*rhs.y - y*rhs.x
        };
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
        rhs.data[0] * scale,
        rhs.data[1] * scale,
        rhs.data[2] * scale
    };
}

template<typename T>
Vector3<T> operator/( T divisor, const Vector3<T>& rhs ) {
    T scale = 1 / divisor;
    return Vector3<T>{
        rhs.data[0] * scale,
        rhs.data[1] * scale,
        rhs.data[2] * scale
    };
}

template<typename T>
struct Vector4: public BaseVector<T, Vector4<T>, 4> {
    Vector4( const Vector3<T>& v, T w )
    :   data{ v.x, v.y, v.z, w }
        {}
    Vector4( std::initializer_list<T> l ) {
        int i = 0;
        for ( float e: l ) {
            data[i] = e;
            i++;
        }
    }
    Vector4( T x, T y, T z, T w )
    :   data{ x, y, z, w } {}
    Vector4()                               = default;
    Vector4( const Vector4& )               = default;
    Vector4( Vector4&& )                    = default;
    Vector4& operator=( const Vector4& )    = default;
    Vector4& operator=( Vector4&& )         = default;
    virtual ~Vector4()                      = default;
    
    operator Vector3<T>() const {
        return Vector3<T> { data[0], data[1], data[2] };
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

template<typename T>
Vector4<T> operator/( T divisor, const Vector4<T>& rhs ) {
    T scale = 1 / divisor;
    return Vector4<T>{
        rhs.data[0] * scale,
        rhs.data[1] * scale,
        rhs.data[2] * scale,
        rhs.data[3] * scale
    };
}

template<typename T, int N>
struct Vector: public BaseVector<T, Vector<T, N>, N> {
    Vector( std::initializer_list<T> l ) {
        int i = 0;
        for ( float e: l ) {
            data[i] = e;
            i++;
        }
    }
    Vector()                            = default;
    Vector( const Vector& )             = default;
    Vector( Vector&& )                  = default;
    Vector& operator=( const Vector& )  = default;
    Vector& operator=( Vector&& )       = default;
    virtual ~Vector()                   = default;
    
    T data[N];
};

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector4f = Vector4<float>;
using Vector2d = Vector2<double>;
using Vector3d = Vector3<double>;
using Vector4d = Vector4<double>;
template<int N> using Vectorf = Vector<float, N>;
template<int N> using Vectord = Vector<double, N>;


}   // math
}   // ce


