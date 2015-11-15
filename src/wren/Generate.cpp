#include "wren/Include.h"
#include "utils/Random.h"
#include "math/Vector.h"
#include "wren/WrenVector.h"
#include <cmath>

namespace pg {
namespace wren {

void BindMathModule( wrenly::Wren& w ) {
    w.beginModule( "builtin/math" )
        .beginClass( "Math" )
            .registerFunction< decltype(&cos), &cos     >( true, "cos(_)" )
            .registerFunction< decltype(&sin), &sin     >( true, "sin(_)" )
            .registerFunction< decltype(&tan), &tan     >( true, "tan(_)" )
            .registerFunction< decltype(&acos), &acos   >( true, "acos(_)" )
            .registerFunction< decltype(&asin), &asin   >( true, "asin(_)" )
            .registerFunction< decltype(&atan), &atan   >( true, "atan(_)" )
            .registerFunction< decltype(&atan2), &atan2 >( true, "atan2(_)" )
            .registerFunction< decltype(&exp), &exp     >( true, "exp(_)" )
            .registerFunction< decltype(&log), &log     >( true, "log(_)" )
            .registerFunction< decltype(&pow), &pow     >( true, "pow(_)" )
            .registerFunction< decltype(&sqrt), &sqrt   >( true, "sqrt(_)" )
            .registerFunction< decltype(&cbrt), &cbrt   >( true, "cbrt(_)" )
            .registerFunction< decltype(&ceil), &ceil   >( true, "ceil(_)" )
            .registerFunction< decltype(&floor), &floor >( true, "floor(_)" )
            .registerFunction< decltype(&round), &round >( true, "round(_)" )
            .registerFunction< decltype(&abs),  &abs    >( true, "abs(_)" )
            .registerFunction< decltype(static_cast<double(*)(double, double)>(&pg::Randd) ), static_cast<double(*)(double, double)>(&pg::Randd) >( true, "rand(_,_)" )
            .registerFunction< decltype(static_cast<double(*)(void)>(&pg::Randd)), static_cast<double(*)(void)>(&pg::Randd) >( true, "rand()" );
}

void BindVectorModule( wrenly::Wren& w ) {
    w.beginModule( "builtin/vector" )
        .registerClass< math::Vector3f, float, float, float >( "Vec3" )
            .registerMethod< decltype(&math::Vector3f::norm), &math::Vector3f::norm >( false, "norm()" )
            .registerMethod< decltype(&math::Vector3f::normSquared), &math::Vector3f::normSquared >( false, "normSquared()" )
            .registerMethod< decltype(&math::Vector3f::dot), &math::Vector3f::dot >( false, "dot(_)" )
            .registerCFunction( false, "cross(_)", wren::cross3f )
            .registerCFunction( false, "plus(_)", wren::plus3f )
            .registerCFunction( false, "minus(_)", wren::minus3f )
        .endClass()
    .endModule();
}

}
}
