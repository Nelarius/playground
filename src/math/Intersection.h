#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Geometry.h"
#include <limits>
#include <cmath>
#include <utility>

namespace pg {
namespace math {

inline bool rayIntersectsAABox(Ray& ray, const AABox& aabb, const Vec3f& aabbPos, const Quatf& aabbQuat, const Vec3f& aabbScale) {
    // the transform matrix's scale numbers have to be one!
    float tmin = 0.0f;
    float tmax = std::numeric_limits<float>::max();

    const float Epsilon{ 0.001f };
    Vec3f delta = aabbPos - ray.origin;

    Vec3f min = aabbScale.hadamard(aabb.min);
    Vec3f max = aabbScale.hadamard(aabb.max);
    const Quatf& q = aabbQuat;
    const Vec3f& s = aabbScale;
    // test intersection with the 2 planes perpendicular to the OBB's x axis
    {
        Vec3f xaxis{
            1.f - 2.f*(q.v.y*q.v.y + q.v.z*q.v.z),
            2.f*(q.v.x*q.v.y + q.w*q.v.z),
            2.f*(q.v.x*q.v.z - q.w*q.v.y)
        };
        xaxis.normalize();
        float e = xaxis.dot(delta);
        float f = ray.direction.dot(xaxis);
        float invF = 1.f / f;

        if (f > Epsilon || f < -Epsilon) {
            // this should be comparable with n.dot(p - p0) / n.dot(direction)
            // but since n (xaxis) is parallel to aabb.min.x, we don't need the
            // dot product
            float t1 = (e + min.x) * invF;  // the division can be performed later!
            float t2 = (e + max.x) * invF;

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
            if (-e - xaxis.x*min.x > 0.f || -e - xaxis.x * max.x < 0.f) {
                return false;
            }
        }

        {
            Vec3f yaxis{
                2.f*(q.v.x*q.v.y - q.w*q.v.z),
                1.f - 2.f*(q.v.x*q.v.x + q.v.z*q.v.z),
                2.f*(q.v.y*q.v.z + q.w*q.v.x)
            };
            yaxis.normalize();
            float e = yaxis.dot(delta);
            float f = ray.direction.dot(yaxis);
            float invF = 1.f / f;

            if (f > Epsilon || f < -Epsilon) {
                float t1 = (e + min.y) * invF;
                float t2 = (e + max.y) * invF;

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
                if (-e - yaxis.y * min.y > 0.f || -e - yaxis.y * max.y < 0.f) {
                    return false;
                }
            }
        }

        {
            Vec3f zaxis{
                2.f*(q.v.x*q.v.z + q.w*q.v.y),
                2.f*(q.v.y*q.v.z - q.w*q.v.x),
                1.f - 2.f*(q.v.x*q.v.x + q.v.y*q.v.y)
            };
            zaxis.normalize();
            float e = zaxis.dot(delta);
            float f = ray.direction.dot(zaxis);
            float invF = 1.f / f;

            if (f > Epsilon || f < -Epsilon) {
                float t1 = (e + min.z) * invF;
                float t2 = (e + max.z) * invF;

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
                if (-e - zaxis.z * min.z > 0.f || -e - zaxis.z * max.z < 0.f) {
                    return false;
                }
            }
        }

        ray.t = tmin;
        return true;
    }
}

}
}
