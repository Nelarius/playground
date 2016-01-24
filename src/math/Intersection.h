#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Geometry.h"
#include <limits>
#include <cmath>
#include <utility>

namespace pg {
namespace math {

// TODO: this assumes that the scale is 1.0 in all axes!! This needs to take the 
// individual transform vectors in instead of the composite matrix
inline bool rayIntersectsAABox(const Ray& ray, const AABox& aabb, const Matrix4f& transform) {
    // the transform matrix's scale numbers have to be one!
    float tmin = 0.0f;
    float tmax = std::numeric_limits<float>::max();

    const float Epsilon{ 0.001f };
    Vec3f aabbPos{ transform.data[3], transform.data[7], transform.data[11] };
    Vec3f delta = aabbPos - ray.origin;

    // test intersection with the 2 planes perpendicular to the OBB's x axis
    {
        Vec3f xaxis{transform.data[0], transform.data[4], transform.data[8]};
        xaxis.normalize();
        float e = xaxis.dot(delta);
        float f = ray.direction.dot(xaxis);
        float invF = 1.f / f;

        if (f > Epsilon || f < -Epsilon) {
            // this should be comparable with n.dot(p - p0) / n.dot(direction)
            // but since n (xaxis) is parallel to aabb.min.x, we don't need the
            // dot product
            float t1 = (e + aabb.min.x) * invF;  // the division can be performed later!
            float t2 = (e + aabb.max.x) * invF;

            // we want t1 to represent the nearest intersection, so if that's
            // not the case, then swap them!
            if (t1 > t2) {
                std::swap(t1, t2);
            }

            if (t2 < tmax) {
                tmax = t2;
            }
            if (t1 > tmin) {
                tmin = t1;
            }
            if (tmax < tmin) {
                return false;
            }
        }
        else {
            // here, the ray is almost parallel to the planes
            // here we check whether the ray is on the inside or outside of the face
            if (-e - xaxis.x*aabb.min.x > 0.f || -e - xaxis.x * aabb.max.x < 0.f) {
                return false;
            }
        }

        {
            Vec3f yaxis{transform.data[1], transform.data[5], transform.data[9]};
            yaxis.normalize();
            float e = yaxis.dot(delta);
            float f = ray.direction.dot(yaxis);
            float invF = 1.f / f;

            if (f > Epsilon || f < -Epsilon) {
                float t1 = (e + aabb.min.y) * invF;
                float t2 = (e + aabb.max.y) * invF;

                if (t1 > t2) {
                    std::swap(t1, t2);
                }

                if (t2 < tmax) {
                    tmax = t2;
                }
                if (t1 > tmin) {
                    tmin = t1;
                }
                if (tmax < tmin) {
                    return false;
                }
            }
            else {
                if (-e - yaxis.y * aabb.min.y > 0.f || -e - yaxis.y * aabb.max.y < 0.f) {
                    return false;
                }
            }
        }

        {
            Vec3f zaxis{transform.data[2], transform.data[6], transform.data[10]};
            zaxis.normalize();
            float e = zaxis.dot(delta);
            float f = ray.direction.dot(zaxis);
            float invF = 1.f / f;

            if (f > Epsilon || f < -Epsilon) {
                float t1 = (e + aabb.min.z) * invF;
                float t2 = (e + aabb.max.z) * invF;

                if (t1 > t2) {
                    std::swap(t1, t2);
                }
                if (t2 < tmax) {
                    tmax = t2;
                }
                if (t1 > tmin) {
                    tmin = t1;
                }
                if (tmax < tmin) {
                    return false;
                }
            }
            else {
                if (-e - zaxis.z * aabb.min.z > 0.f || -e - zaxis.z * aabb.max.z < 0.f) {
                    return false;
                }
            }
        }

        return true;
    }
}

}
}
