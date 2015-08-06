#include <utils/Random.h>
#include <random>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

// hide the implementation details
namespace {

std::minstd_rand& global_urng() {
    static std::minstd_rand u;
    return u;
}

}

namespace pg {

void Randomize() {
    std::random_device rd{};
    global_urng().seed( rd() );
}

std::int32_t Randi( std::int32_t a, std::int32_t b ) {
    static std::uniform_int_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d( global_urng(), parm_t{ a, b } );
}

float Randf( float a, float b ) {
    static std::uniform_real_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d( global_urng(), parm_t{ a, b } );
}

float Randf() {
    return Randf( 0.0f, 1.0f );
}

double Randd( double a, double b ) {
    static std::uniform_real_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d( global_urng(), parm_t{ a, b } );
}


}