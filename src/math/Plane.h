
#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"

namespace ce {

template<typename T>
class Plane {
    public:
        Plane( const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3 )
        :   point_( p1 ),
            first_( p1 - p2 ),
            second_( p3 - p2 ),
            normal_() {
            normal_ = first_.cross( second_ ).normalize();
        }
        Plane()     = default;
        ~Plane()    = default;
        
        /**
         * @brief Check if the point is on the positive or negative side of the plane.
         * @param v
         * @return true, if on the positive side, false otherwise.
         */
        bool isOnPositiveSide( const Vector3<T>& v ) const {
            ce::Matrix3<T> m{ v, first_, second_ };
            if ( m.determinant() > 0.0f ) {
                return true;
            }
            return false;
        }
        /**
         * @brief Check if the point is on the positive or negative side of the plane.
         * @param v
         * @return true, if on the negative side, false otherwise.
         */
        bool isOnNegativeSide( const Vector3<T>& v ) const {
            ce::Matrix3<T> m{ v, first_, second_ };
            if ( m.determinant() < 0.0f ) {
                return true;
            }
            return false;
        }
        /**
         * @brief Return the perpendicular distance from a point to this plane.
         * @param v 
         * @return 
         */
        T distanceToPlane( const Vector3<T>& v ) const { return normal_.dot( v ) - constant(); }
        /**
         * @brief Get the plane normal.
         */
        Vector3<T> normal() const { return normal_; }
        /**
         * @brief Get the plane constant.
         * @return The dot product of the plane normal with a point on the plane.
         */
        T constant() const { return point_.dot( normal_ ); }
        
    private:
        Vector3<T>    point_{};
        Vector3<T>    first_{};
        Vector3<T>    second_{};
        Vector3<T>    normal_{};
};

using Planef = Plane<float>;
using Planed = Plane<double>;

}

