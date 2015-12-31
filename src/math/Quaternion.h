
#pragma once

#include "math/Vector.h"
#include <cmath>

namespace pg {
namespace math {

template<typename T> 
class Quaternion {
    
    public:
        Quaternion()                                = default;
        Quaternion( const Quaternion& )             = default;
        Quaternion( Quaternion&& )                  = default;
        Quaternion& operator=( const Quaternion& )  = default;
        Quaternion& operator=( Quaternion&& )       = default;
        
        static Quaternion<T> Identity() {
            return Quaternion<T>{ 0.0, 0.0f, 0.0f, 1.0f };
        }
        
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
            T factor = 1.0f / normSquared();
            return conjugate() * factor;
        }
        
        T norm() const {
            return sqrt( v.normSquared() + w*w );
        }
        
        T normSquared() const {
            return v.normSquared() + w*w;
        }
        
        Quaternion<T> operator*( const Quaternion& rhs ) const {
            return Quaternion<T> {
                v.cross( rhs.v ) + rhs.w*v + w*rhs.v,
                w*rhs.w - v.dot( rhs.v )
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
        
        Quaternion<T> multiply( const Quaternion& rhs ) const {
            return Quaternion<T> {
                v.cross( rhs.v ) + rhs.w*v + w*rhs.v,
                w*rhs.w - v.dot( rhs.v )
            };
        }
        
        Vector3<T> axis() const {
            T angle = this->angle();
            return v * ( static_cast<T>(1.0) / sin( angle / static_cast<T>(2.0) ) );
        }
        
        T angle() const {
            return static_cast<T>(2.0 * acos( w ));
        }
        
        Vector3<T>  v{};    // the imaginary part
        T           w{ static_cast<T>(1.0) }; // the real part
};

template<typename T>
Quaternion<T> operator*( T lhs, const Quaternion<T>& rhs ) {
    return Quaternion<T>{ lhs*rhs.v.x, lhs*rhs.v.y, lhs*rhs.v.z, lhs*rhs.w };
}

using Quatf = Quaternion<float>;

}   // math
}   // pg
