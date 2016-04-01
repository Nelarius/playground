#include "Log.h"

namespace pg {

std::ostream& operator<<(std::ostream& os, const math::Vec2f& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const math::Vec3f& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const math::Vec4f& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const math::Quatf& q) {
    os << "[(" << q.v.x << ", " << q.v.y << ", " << q.v.z << "), " << q.w << "]";
    return os;
}

}
