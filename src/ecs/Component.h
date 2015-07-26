
#pragma once

#include "ecs/TypeCount.h"
#include <cstdint>

namespace ce {
namespace ecs {

template<typename T>
class Component: public TypeCount {
    public:
        static uint32_t family() {
            static uint32_t f{ familyCounter_++ };
            return f;
        }
};

}   // namespace ecs
}   // namespace ce

