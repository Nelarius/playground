
#pragma once

#include <cstdint>

namespace pg {
namespace ecs {

class BaseComponent {
    protected:    
        static uint32_t familyCounter_;
};
    
template<typename T>
class Component: public BaseComponent {
    public:
        static uint32_t family() {
            static uint32_t f{ familyCounter_++ };
            return f;
        }
};

}   // namespace ecs
}   // namespace ce

