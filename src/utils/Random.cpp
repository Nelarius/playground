#include <utils/Random.h>
#include <random>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

namespace {

std::minstd_rand& global_urng() {
    static std::minstd_rand u;
    return u;
}

}

namespace pg {

void randomize() {
    std::random_device rd{};
    global_urng().seed(rd());
}

std::int32_t randi(std::int32_t a, std::int32_t b) {
    static std::uniform_int_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(global_urng(), parm_t{ a, b });
}

float randf(float a, float b) {
    static std::uniform_real_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return float(d(global_urng(), parm_t{ a, b }));
}

float randf() {
    return randf(0.0f, 1.0f);
}

double randd(double a, double b) {
    static std::uniform_real_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(global_urng(), parm_t{ a, b });
}

double randd() {
    return randd(0.0, 1.0);
}

}