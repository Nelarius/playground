#include "wren/WrenVector.h"

namespace pg {
namespace wren {

void cross3f( WrenVM* vm ) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 0 );
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 1 );
    math::Vector3f res = lhs->cross( *rhs );
    WrenValue* ret = nullptr;
    WrenValue* constructor  = wrenGetMethod( vm, "builtin/vector", "createVector3", "call(_,_,_)" );
    wrenCall( vm, constructor, &ret, "ddd", res.x, res.y, res.z );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

void plus3f( WrenVM* vm ) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 0 );
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 1 );
    math::Vector3f res = lhs->operator+( *rhs );
    WrenValue* ret = nullptr;
    WrenValue* constructor = wrenGetMethod( vm, "builtin/vector", "createVector3", "call(_, _, _)" );
    wrenCall( vm, constructor, &ret, "ddd", res.x, res.y, res.z );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

void minus3f( WrenVM* vm ) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 0 );
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 1 );
    math::Vector3f res = lhs->operator-( *rhs );
    WrenValue* ret = nullptr;
    WrenValue* constructor = wrenGetMethod( vm, "builtin/vector", "createVector3", "call(_,_,_)" );
    wrenCall( vm, constructor, &ret, "ddd", res.x, res.y, res.z );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

void hadamard3f( WrenVM* vm ) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 0 );
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetArgumentForeign( vm, 1 );
    math::Vector3f res = lhs->hadamard( *rhs );
    WrenValue* ret = nullptr;
    WrenValue* constructor = wrenGetMethod( vm, "builtin/vector", "createVector3", "call(_,_,_)" );
    wrenCall( vm, constructor, &ret, "ddd", res.x, res.y, res.z );
    wrenReturnValue( vm, ret );
    wrenReleaseValue( vm, ret );
}

}
}