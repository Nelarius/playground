#include "wren/WrenVector.h"

namespace pg {
namespace wren {

void cross3f(WrenVM* vm) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 0);
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 1);
    math::Vector3f res = lhs->cross(*rhs);
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

void plus3f(WrenVM* vm) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 0);
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 1);
    math::Vector3f res = lhs->operator+(*rhs);
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

void minus3f(WrenVM* vm) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 0);
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 1);
    math::Vector3f res = lhs->operator-(*rhs);
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

void hadamard3f(WrenVM* vm) {
    const math::Vector3f* lhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 0);
    const math::Vector3f* rhs = (const math::Vector3f*)wrenGetSlotForeign(vm, 1);
    math::Vector3f res = lhs->hadamard(*rhs);
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