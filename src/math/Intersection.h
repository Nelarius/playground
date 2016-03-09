#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Geometry.h"
#include <limits>
#include <cmath>
#include <utility>

namespace pg {
namespace math {

inline void rayIntersectsPlane(Rayf& ray, const Planef& plane) {
    PG_ASSERT(plane.normal().norm() == 1.f);
    float intersectParam = (plane.point() - ray.origin).dot(plane.normal()) / ray.direction.dot(plane.normal());
    ray.t = intersectParam;
}

inline bool rayIntersectsSphere(Rayf& ray, const Spheref& sphere) {
    Vec3f oc = ray.origin - sphere.center;
    float loc = ray.direction.dot(oc);
    float ocNorm = oc.norm();
    float discriminant = loc*loc - ocNorm*ocNorm + sphere.radius*sphere.radius;
    // no real solution exists, no intersection
    if (discriminant < 0.f) {
        return false;
    }
    // two solutions exist, take the smaller one
    else if (discriminant > 0.f) {
        float discriminantSqrt = sqrt(discriminant);
        float d1 = -loc + discriminantSqrt;
        float d2 = -loc - discriminantSqrt;
        ray.t = std::min(d1, d2);
        return true;
    }

    // only one solution exists
    ray.t = -loc;
    return true;
}

inline bool rayIntersectsAABox(Rayf& ray, const AABoxf& aabb, const Vec3f& aabbPos, const Quatf& aabbQuat, const Vec3f& aabbScale) {
    // for calculating the transformed coordinate system

    float n = 2.f / aabbQuat.norm();
    // the transform matrix's scale numbers have to be one!
    float tmin = -std::numeric_limits<float>::max();
    float tmax = std::numeric_limits<float>::max();

    const float Epsilon{ 0.001f };
    Vec3f delta = (aabbPos - ray.origin);

    Vec3f min = aabbScale.hadamard(aabb.min);
    Vec3f max = aabbScale.hadamard(aabb.max);
    const Quatf& q = aabbQuat;
    // test intersection with the 2 planes perpendicular to the OBB's x axis
    {
        Vec3f xaxis{
            1.f - n*(q.v.y*q.v.y + q.v.z*q.v.z),
            n*(q.v.x*q.v.y + q.w*q.v.z),
            n*(q.v.x*q.v.z - q.w*q.v.y)
        };
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
            // we check whether the ray is on the inside or outside of the two boundary planes
            if (-e - xaxis.x*min.x > 0.f || -e - xaxis.x * max.x < 0.f) {
                return false;
            }
        }

        {
            Vec3f yaxis{
                n*(q.v.x*q.v.y - q.w*q.v.z),
                1.f - n*(q.v.x*q.v.x + q.v.z*q.v.z),
                n*(q.v.y*q.v.z + q.w*q.v.x)
            };
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
                n*(q.v.x*q.v.z + q.w*q.v.y),
                n*(q.v.y*q.v.z - q.w*q.v.x),
                1.f - n*(q.v.x*q.v.x + q.v.y*q.v.y)
            };
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

        ray.t = float(tmin);
        return true;
    }
}

}
}
