
#pragma once

#include <type_traits>
#include <cstdint>

namespace pg {
namespace ecs {
namespace detail {

inline uint32_t& componentId() {
    static uint32_t id{ 0u };
    return id;
}

template<typename T>
uint32_t getComponentIdImpl() {
    static uint32_t id = ++componentId();
    return id;
}

}   // detail

template<typename C>
uint32_t getComponentId() {
    return detail::getComponentIdImpl<std::decay_t<C>>();
}

}
}
