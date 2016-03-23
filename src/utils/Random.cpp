#include <utils/Random.h>
#include <random>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

namespace {

std::minstd_rand& globalUniformRng() {
    static std::minstd_rand u;
    return u;
}

}

namespace pg {

void randomize() {
    std::random_device rd{};
    globalUniformRng().seed(rd());
}

void seed(unsigned int s) {
    globalUniformRng().seed(s);
}

std::int32_t randi(std::int32_t a, std::int32_t b) {
    static std::uniform_int_distribution<> d{};
    using Distribution = decltype(d)::param_type;
    return d(globalUniformRng(), Distribution{ a, b });
}

float randf(float a, float b) {
    static std::uniform_real_distribution<float> d{};
    using Distribution = decltype(d)::param_type;
    return d(globalUniformRng(), Distribution{ a, b });
}

float randf() {
    return randf(0.0f, 1.0f);
}

double randd(double a, double b) {
    static std::uniform_real_distribution<double> d{};
    using Distribution = decltype(d)::param_type;
    return d(globalUniformRng(), Distribution{ a, b });
}

double randd() {
    return randd(0.0, 1.0);
}

}