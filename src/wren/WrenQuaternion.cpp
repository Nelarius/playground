#include "wren/WrenQuaternion.h"
#include "math/Quaternion.h"

namespace pg {
namespace wren {

void conjugate( WrenVM* vm ) {
    const math::Quatf* q = (const math::Quatf*)wrenGetArgumentForeign( vm, 0 );
    math::Quatf res = q->conjugate();
    WrenValue* ret = nullptr;
    WrenValue* constructor = wrenGetMethod( vm, "builtin/quaternion", "createQuaternion", "call(_,_,_,_)" );
    wrenCall( vm, constructor, &ret, "dddd", res.v.x, res.v.y, res.v.z, res.w );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

void inverse( WrenVM* vm ) {
    const math::Quatf* q = (const math::Quatf*)wrenGetArgumentForeign( vm, 0 );
    math::Quatf res = q->inverse();
    WrenValue* ret = nullptr;
    WrenValue* constructor = wrenGetMethod( vm, "builtin/quaternion", "createQuaternion", "call(_,_,_,_)" );
    wrenCall( vm, constructor, &ret, "dddd", res.v.x, res.v.y, res.v.z, res.w );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

void multiply( WrenVM* vm ) {
    const math::Quatf* lhs = (const math::Quatf*)wrenGetArgumentForeign( vm, 0 );
    const math::Quatf* rhs = (const math::Quatf*)wrenGetArgumentForeign( vm, 1 );
    math::Quatf res = lhs->multiply( *rhs );
    WrenValue* ret = nullptr;
    WrenValue* constructor = wrenGetMethod( vm, "builtin/quaternion", "createQuaternion", "call(_,_,_,_)" );
    wrenCall( vm, constructor, &ret, "dddd", res.v.x, res.v.y, res.v.z, res.w );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

void axis( WrenVM* vm ) {
    const math::Quatf* q = (const math::Quatf*)wrenGetArgumentForeign( vm, 0 );
    math::Vector3f res = q->axis();
    WrenValue* ret = nullptr;
    WrenValue* constructor = wrenGetMethod( vm, "builtin/vector", "createVector3", "call(_,_,_)" );
    wrenCall( vm, constructor, &ret, "ddd", res.x, res.y, res.z );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

}
}