
#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include <cmath>

namespace pg {
namespace math {

template<typename T> 
class Quaternion {
    public:
        Quaternion()    = default;
        Quaternion( const Vector3<T>& i, T r )
        :   v( i ),
            w( r )
            {}
        Quaternion( T x, T y, T z, T w )
        :   v( x, y, z ),
            w( w )
            {}
            
        Quaternion<T> conjugate() const {
            return Quaternion<T>{ -v.x, -v.y, -v.z, w };
        }
        
        Quaternion<T> inverse() const {
            T factor = 1.0 / normSquared();
            return conjugate() * factor;
        }
        
        T norm() const {
            return sqrt( v.squaredLength() + w*w );
        }
        
        T normSquared() const {
            return v.squaredLength() + w*w;
        }
        
        Quaternion<T> operator*( const Quaternion& rhs ) const {
            return Quaternion<T> {
                v.cross( rhs.v ) + rhs.w*v + w*rhs.v,
                w*rhs*w - v.dot( rhs.v )
            };
        }
        
        Quaternion<T> operator*( T rhs ) const {
            return Quaternion<T> {
                rhs*v.x,
                rhs*v.y,
                rhs*v.z,
                rhs*w
            };
        }
        
        Vector3<T>  v{};    // the imaginary part
        T           w{ 1 }; // the real part
};

template<typename T>
Quaternion<T> operator*( T lhs, const Quaternion<T>& rhs ) {
    return Quaternion<T>{ lhs*rhs.v.x, lhs*rhs.v.y, lhs*rhs.v.z, lhs*rhs.w };
}

}   // math
}   // pg
