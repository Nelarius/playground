#include "wren/Include.h"
#include "utils/Random.h"
#include <cmath>

namespace pg {
namespace wren {
    
void GenerateMathModule( wrenly::Wren& w ) {
    w.beginModule( "math" )
        .beginClass( "Math" )
            .registerFunction< decltype(&cos), &cos     >( true, "cos(_)" )
            .registerFunction< decltype(&sin), &sin     >( true, "sin(_)" )
            .registerFunction< decltype(&tan), &tan     >( true, "tan()_" )
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

void GenerateVectorModule( wrenly::Wren& w ) {
    //
}
    
}
}
