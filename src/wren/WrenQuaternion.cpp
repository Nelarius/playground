#include "wren/WrenQuaternion.h"
#include "math/Quaternion.h"

namespace pg {
namespace wren {

void conjugate( WrenVM* vm ) {
    const math::Quatf* q = (const math::Quatf*)wrenGetSlotForeign( vm, 0 );
    math::Quatf res = q->conjugate();
    WrenValue* function = wrenGetSlotValue(vm, 1);
    WrenValue* handle = wrenMakeCallHandle(vm, "call_,_,_,_");
    wrenSetSlotValue(vm, 0, function);
    wrenSetSlotDouble(vm, 1, res.v.x);
    wrenSetSlotDouble(vm, 2, res.v.y);
    wrenSetSlotDouble(vm, 3, res.v.z);
    wrenSetSlotDouble(vm, 4, res.w);
    wrenCall(vm, handle);
    wrenReleaseValue(vm, function);
    wrenReleaseValue(vm, handle);
}

void inverse( WrenVM* vm ) {
    const math::Quatf* q = (const math::Quatf*)wrenGetSlotForeign( vm, 0 );
    math::Quatf res = q->inverse();
    WrenValue* function = wrenGetSlotValue(vm, 1);
    WrenValue* handle = wrenMakeCallHandle(vm, "call_,_,_,_");
    wrenSetSlotValue(vm, 0, function);
    wrenSetSlotDouble(vm, 1, res.v.x);
    wrenSetSlotDouble(vm, 2, res.v.y);
    wrenSetSlotDouble(vm, 3, res.v.z);
    wrenSetSlotDouble(vm, 4, res.w);
    wrenCall(vm, handle);
    wrenReleaseValue(vm, function);
    wrenReleaseValue(vm, handle);
}

void multiply( WrenVM* vm ) {
    const math::Quatf* lhs = (const math::Quatf*)wrenGetSlotForeign( vm, 0 );
    const math::Quatf* rhs = (const math::Quatf*)wrenGetSlotForeign( vm, 1 );
    wrenEnsureSlots( vm, 5 );
    math::Quatf res = lhs->multiply( *rhs );
    WrenValue* function = wrenGetSlotValue(vm, 1);
    WrenValue* handle = wrenMakeCallHandle(vm, "call_,_,_,_");
    wrenSetSlotValue(vm, 0, function);
    wrenSetSlotDouble(vm, 1, res.v.x);
    wrenSetSlotDouble(vm, 2, res.v.y);
    wrenSetSlotDouble(vm, 3, res.v.z);
    wrenSetSlotDouble(vm, 4, res.w);
    wrenCall(vm, handle);
    wrenReleaseValue(vm, function);
    wrenReleaseValue(vm, handle);
}

void axis( WrenVM* vm ) {
    const math::Quatf* q = (const math::Quatf*)wrenGetSlotForeign( vm, 0 );
    math::Vector3f res = q->axis();
    WrenValue* function = wrenGetSlotValue(vm, 1);
    WrenValue* handle = wrenMakeCallHandle(vm, "call_,_,_");
    wrenSetSlotValue(vm, 0, function);
    wrenSetSlotDouble(vm, 1, res.x);
    wrenSetSlotDouble(vm, 2, res.y);
    wrenSetSlotDouble(vm, 3, res.z);
    wrenCall(vm, handle);
    wrenReleaseValue(vm, function);
    wrenReleaseValue(vm, handle);
}

}
}