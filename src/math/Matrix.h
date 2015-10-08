
#pragma once

#include "math/Vector.h"
#include "math/Quaternion.h"
#include <cmath>

namespace pg {
namespace math {

template<typename T>
struct Matrix2 {
    T data[4];
    
    Matrix2()
    :   data{ 1.0, 0.0, 0.0, 1.0 }
        {}
        
    Matrix2( std::initializer_list<T> l ) {
        int i = 0;
        for ( T e: l ) {
            data[i] = e;
            i++;
        }
    }
    
    Matrix2( const Vector2<T>& r1, const Vector2<T>& r2 )
    :   data{ r1.x, r1.y, r2.x, r2.y }
        {}
        
    T trace() const {
        return data[0] + data[3];
    }
    
    Vector2<T> operator*( const Vector2<T>& v ) {
        return Vector2<T> {
            data[0]*v.x + data[1]*v.y,
            data[2]*v.y + data[3]*v.y
        };
    }
    
    Matrix2<T> operator*( const Matrix2<T>& m ) {
        return Matrix2<T> {
            data[0]*m.data[0] + data[1]*m.data[2],
            data[0]*m.data[1] + data[1]*m.data[3],
            data[2]*m.data[0] + data[3]*m.data[2],
            data[2]*m.data[1] + data[3]*m.data[3]
        };
    }
    
    Matrix2<T> operator*( T val ) const {
        return Matrix2<T> {
            val*data[0], val*data[1],
            val*data[2], val*data[3]
        };
    }
    
    Matrix2<T> operator-( const Matrix2<T>& m ) const {
        return Matrix2<T> {
            data[0] - m.data[0], data[1] - m.data[1],
            data[2] - m.data[2], data[3] - m.data[3]
        };
    }
    
    Matrix2<T> operator+( const Matrix2<T>& m ) const {
        return Matrix2<T> {
            data[0] + m.data[0], data[1] + m.data[1],
            data[2] + m.data[2], data[3] + m.data[3]
        };
    }
    
    Matrix2<T> transpose() const {
        return Matrix2<T> {
            data[0], data[2],
            data[1], data[3]
        };
    }
    
    T determinant() const {
        return data[0]*data[3] - data[2]*data[1];
    }
    
    Matrix2<T> inverse() const {
        // obtained using the Cayley-Hamilton method
        T factor = 1.0 / determinant();
        return factor * ( trace()*Matrix2<T>{} - *this );
    }
};

template<typename T>
Matrix2<T> operator*( T val, const Matrix2<T>&  m ) {
    return Matrix2<T> {
        val*m.data[0], val*m.data[1],
        val*m.data[2], val*m.data[3]
    };
}
    
template<typename T>
struct Matrix3 {
        T data[9];
        
        Matrix3( std::initializer_list<T> l ) {
            int i = 0;
            for ( T e: l ) {
                data[i] = e;
                i++;
            }
        }
        Matrix3()
        :   data{ 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 }
            {}
            
        Matrix3( const Vector3<T>& r1, const Vector3<T>& r2, const Vector3<T>& r3 )
        :   data{ r1.x, r1.y, r1.z, r2.x, r2.y, r2.z, r3.x, r3.y, r3.z }
            {}
            
        T trace() const {
            return data[0] + data[4] + data[8];
        }
    
        Vector3<T> operator*( const Vector3<T>& v ) const {
            return Vector3<T> {
                data[0]*v.x + data[1]*v.y + data[2]*v.z,
                data[3]*v.x + data[4]*v.y + data[5]*v.z,
                data[6]*v.x + data[7]*v.y + data[8]*v.z
            };
        }
        
        Matrix3<T> operator*( const Matrix3<T>& m ) const {
            return Matrix3<T> {
                data[0]*m.data[0] + data[1]*m.data[3] + data[2]*m.data[6],
                data[0]*m.data[1] + data[1]*m.data[4] + data[2]*m.data[7],
                data[0]*m.data[2] + data[1]*m.data[5] + data[2]*m.data[8],
                data[3]*m.data[0] + data[4]*m.data[3] + data[5]*m.data[6],
                data[3]*m.data[1] + data[4]*m.data[4] + data[5]*m.data[7],
                data[3]*m.data[2] + data[4]*m.data[5] + data[5]*m.data[8],
                data[6]*m.data[0] + data[7]*m.data[3] + data[8]*m.data[6],
                data[6]*m.data[1] + data[7]*m.data[4] + data[8]*m.data[7],
                data[6]*m.data[2] + data[7]*m.data[5] + data[8]*m.data[8]
            };
        }
        
        Matrix3<T> operator*( T val ) const {
            return Matrix3<T> {
                val*data[0], val*data[1], val*data[2],
                val*data[3], val*data[4], val*data[5],
                val*data[6], val*data[7], val*data[8]
            };
        }
        
        Matrix3<T> operator+( const Matrix3<T>& m ) const {
            return Matrix3<T> {
                data[0] + m.data[0], data[1] + m.data[1], data[2] + m.data[2], 
                data[3] + m.data[3], data[4] + m.data[4], data[5] + m.data[5], 
                data[6] + m.data[6], data[7] + m.data[7], data[8] + m.data[8]
            };
        }
        
        Matrix3<T> operator-( const Matrix3<T>& m ) const {
            return Matrix3<T> {
                data[0] - m.data[0], data[1] - m.data[1], data[2] - m.data[2], 
                data[3] - m.data[3], data[4] - m.data[4], data[5] - m.data[5], 
                data[6] - m.data[6], data[7] - m.data[7], data[8] - m.data[8]
            };
        }
        
        Matrix3<T> transpose() const {
            return Matrix3<T> {
                data[0], data[3], data[6], 
                data[1], data[4], data[7], 
                data[2], data[5], data[8]
            };
        }
        
        T determinant() const {
            return 
                  data[0]*Matrix2<T>{ data[4], data[5], data[7], data[8] }.determinant() 
                - data[1]*Matrix2<T>{ data[3], data[5], data[6], data[8] }.determinant() 
                + data[2]*Matrix2<T>{ data[3], data[4], data[6], data[7] }.determinant();
        }
        
        Matrix3<T> inverse() const {
            // obtained using the Cayley-Hamilton method
            T factor = 1.0 / determinant();
            const Matrix3<T>& A = *this;
            Matrix3<T> AA = A*A;
            T trA = A.trace();
            return factor * ( Matrix3<T>{}*0.5*(trA*trA - AA.trace()) - A*trA + AA );
        }
};

template<typename T>
Matrix3<T> operator*( T val, const Matrix3<T>& m ) {
    return Matrix3<T> {
        val*m.data[0], val*m.data[1], val*m.data[2],
        val*m.data[3], val*m.data[4], val*m.data[5],
        val*m.data[6], val*m.data[7], val*m.data[8]
    };
}

/**
 * @class Matrix4
 * @author Muszynski Johann M
 * @date 30/06/15
 * @file Matrix.h
 * @brief Four-by-four matrix class.
 * Data stored in row-major order.
 */
template<typename T>
struct Matrix4 {
    T data[16];
    
    Matrix4( std::initializer_list<T> l ) {
        int i = 0;
        for ( T e: l ) {
            data[i] = e;
            i++;
        }
    }
    
    Matrix4()
    :   data{ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }
        {}
        
    Matrix4( const Vector4<T>& r1, const Vector4<T>& r2, const Vector4<T>& r3, const Vector4<T>& r4 )
    :   data{ r1.x, r1.y, r1.z, r1.w, r2.x, r2.y, r2.z, r2.w, r3.x, r3.y, r3.z, r3.w, r4.x, r4.y, r4.z, r4.w }
        {}
    
    static Matrix4<T> Translation( const Vector3<T>& v ) {
        return Matrix4<T>{
                1.0f, 0.0f, 0.0f, v.x,
                0.0f, 1.0f, 0.0f, v.y,
                0.0f, 0.0f, 1.0f, v.z,
                0.0f, 0.0f, 0.0f, 1.0f
        };
    }
    
    static Matrix4<T> Rotation( const Quaternion<T>& q ) {
        T s = 2.0 / q.norm();
        return Matrix4<T> {
            1 - s*(q.v.y*q.v.y + q.v.z*q.v.z),  s*(q.v.x*q.v.y - q.w*q.v.z),        s*(q.v.x*q.v.z + q.w*q.v.y),        0.0,
            s*(q.v.x*q.v.y + q.w*q.v.z),        1 - s*(q.v.x*q.v.x + q.v.z*q.v.z),  s*(q.v.y*q.v.z - q.w*q.v.x),        0.0,
            s*(q.v.x*q.v.z - q.w*q.v.y),        s*(q.v.y*q.v.z + q.w*q.v.x),        1 - s*(q.v.x*q.v.x + q.v.y*q.v.y),  0.0,
            0.0,                                0.0,                                0.0,                                1.0
        };
    }
    
    static Matrix4<T> Scale( const Vector3<T>& s ) {
        return Matrix4<T> {
            s.x, 0.0, 0.0, 0.0,
            0.0, s.y, 0.0, 0.0,
            0.0, 0.0, s.z, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    }
    
    /**
     * @brief Symmetric orthographic projection matrix
     * @param width
     * @param height
     * @param near
     * @param far
     * @return 
     */
    static Matrix4<T> Orthographic( float width, float height, float near, float far ) {
        return Matrix4<T>{
            2.0f/width,     0.0f,           0.0f,                       0.0f,
            0.0f,           2.0f/height,    0.0f,                       0.0f,
            0.0f,           0.0f,           2.0f / (near - far),        (near + far) / (near - far),
            0.0f,           0.0f,           0.0f,                       1.0f
        };
    }
    
    /**
     * @brief Symmetric perspective projection matrix
     * @param vfov The vertical field of view
     * @param ar The aspect ratio of the projection plane
     * @param n The near plane
     * @param f The far plane
     * @return The OpenGL perspective projection matrix
     */
    static Matrix4<T> Perspective( float vfov, float ar, float n, float f ) {
        T h = 2.0f * n * tan( 0.5f * vfov );
        T w = ar * h;
        return Matrix4<T>{
            2.0f*n / w,     0.0f,           0.0f,                   0.0f,
            0.0f,           2.0f*n / h,     0.0f,                   0.0f,
            0.0f,           0.0f,           -(f + n) / (f - n),     -(2.0f*f*n) / (f - n),
            0.0f,           0.0f,           -1.0f,                  0.0f
        };
    }
    
    T trace() const {
        return data[0] + data[5] + data[10] + data[15];
    }
    
    Vector4<T> operator*( const Vector4<T>& v ) const {
        return Vector4<T> {
            data[0]*v.x + data[1]*v.y + data[2]*v.z + data[3]*v.w,
            data[4]*v.x + data[5]*v.y + data[6]*v.z + data[7]*v.w,
            data[8]*v.x + data[9]*v.y + data[10]*v.z + data[11]*v.w,
            data[12]*v.x + data[13]*v.y + data[14]*v.z + data[15]*v.w,
        };
    }
    
    Matrix4<T> operator*( const Matrix4<T>& m ) const {      
        return Matrix4<T> {
            data[0]*m.data[0] + data[1]*m.data[4] + data[2]*m.data[8] + data[3]*m.data[12],
            data[0]*m.data[1] + data[1]*m.data[5] + data[2]*m.data[9] + data[3]*m.data[13],
            data[0]*m.data[2] + data[1]*m.data[6] + data[2]*m.data[10] + data[3]*m.data[14],
            data[0]*m.data[3] + data[1]*m.data[7] + data[2]*m.data[11] + data[3]*m.data[15],
            data[4]*m.data[0] + data[5]*m.data[4] + data[6]*m.data[8] + data[7]*m.data[12],
            data[4]*m.data[1] + data[5]*m.data[5] + data[6]*m.data[9] + data[7]*m.data[13],
            data[4]*m.data[2] + data[5]*m.data[6] + data[6]*m.data[10] + data[7]*m.data[14],
            data[4]*m.data[3] + data[5]*m.data[7] + data[6]*m.data[11] + data[7]*m.data[15],
            data[8]*m.data[0] + data[9]*m.data[4] + data[10]*m.data[8] + data[11]*m.data[12],
            data[8]*m.data[1] + data[9]*m.data[5] + data[10]*m.data[9] + data[11]*m.data[13],
            data[8]*m.data[2] + data[9]*m.data[6] + data[10]*m.data[10] + data[11]*m.data[14],
            data[8]*m.data[3] + data[9]*m.data[7] + data[10]*m.data[11] + data[11]*m.data[15],
            data[12]*m.data[0] + data[13]*m.data[4] + data[14]*m.data[8] + data[15]*m.data[12],
            data[12]*m.data[1] + data[13]*m.data[5] + data[14]*m.data[9] + data[15]*m.data[13],
            data[12]*m.data[2] + data[13]*m.data[6] + data[14]*m.data[10] + data[15]*m.data[14],
            data[12]*m.data[3] + data[13]*m.data[7] + data[14]*m.data[11] + data[15]*m.data[15]
        };
    }
    
    Matrix4<T> operator+( const Matrix4<T>& m ) const {
        return Matrix4<T> {
            data[0] + m.data[0], data[1] + m.data[1], data[2] + m.data[2], data[3] + m.data[3], 
            data[4] + m.data[4], data[5] + m.data[5], data[6] + m.data[6], data[7] + m.data[7], 
            data[8] + m.data[8], data[9] + m.data[9], data[10] + m.data[10], data[11] + m.data[11], 
            data[12] + m.data[12], data[13] + m.data[13], data[14] + m.data[14], data[15] + m.data[15]
        };
    }
    
    Matrix4<T> operator-( const Matrix4<T>& m ) const {
        return Matrix4<T> {
            data[0] - m.data[0], data[1] - m.data[1], data[2] - m.data[2], data[3] - m.data[3], 
            data[4] - m.data[4], data[5] - m.data[5], data[6] - m.data[6], data[7] - m.data[7], 
            data[8] - m.data[8], data[9] - m.data[9], data[10] - m.data[10], data[11] - m.data[11], 
            data[12] - m.data[12], data[13] - m.data[13], data[14] - m.data[14], data[15] - m.data[15]
        };
    }
    
    Matrix4<T> operator*( T val ) const {
        return Matrix4<T> {
        val*data[0], val*data[1], val*data[2], val*data[3],
        val*data[4], val*data[5], val*data[6], val*data[7],
        val*data[8], val*data[9], val*data[10], val*data[11],
        val*data[12], val*data[13], val*data[14], val*data[15]
        };
    }
    
    Matrix4<T> transpose() const {
        return Matrix4<T> {
            data[0], data[4], data[8], data[12],
            data[1], data[5], data[9], data[13],
            data[2], data[6], data[10], data[14],
            data[3], data[7], data[11], data[15]
        };
    }
    
    T determinant() const {
        return
              data[0]*Matrix3<T>{ data[5], data[6], data[7], data[9], data[10], data[11], data[13], data[14], data[15] }.determinant()
            - data[1]*Matrix3<T>{ data[4], data[6], data[7], data[8], data[10], data[11], data[12], data[14], data[15] }.determinant()
            + data[2]*Matrix3<T>{ data[4], data[5], data[7], data[8], data[9], data[11], data[12], data[13], data[15] }.determinant()
            - data[3]*Matrix3<T>{ data[4], data[5], data[6], data[8], data[9], data[10], data[12], data[13], data[14] }.determinant();
    }
    
    Matrix4<T> inverse() const {
        // obtained using the Cayley-Hamilton method
        T factor = 1.0 / determinant();
        const Matrix4<T>& A = *this;
        Matrix4<T> AA = A*A;
        Matrix4<T> AAA = AA*A;
        T trA = trace();
        T trAA = AA.trace();
        T trAAA = AAA.trace();
        return factor * ( Matrix4<T>{}*0.1666666667*(trA*trA*trA - 3.0*trA*trAA + 2.0*trAAA) - A*0.5*(trA*trA - trAA) + AA*trA - AAA );
    }
};

template<typename T>
Matrix4<T> operator*( T val, const Matrix4<T>& m ) {
    return Matrix4<T>{
        val*m.data[0], val*m.data[1], val*m.data[2], val*m.data[3],
        val*m.data[4], val*m.data[5], val*m.data[6], val*m.data[7],
        val*m.data[8], val*m.data[9], val*m.data[10], val*m.data[11],
        val*m.data[12], val*m.data[13], val*m.data[14], val*m.data[15]
    };
}

using Matrix2f = Matrix2<float>;
using Matrix3f = Matrix3<float>;
using Matrix4f = Matrix4<float>;

    
}   // math
}   // ce

