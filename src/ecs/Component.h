
#pragma once

#include "ecs/Id.h"

namespace ce {
namespace ecs {

class BaseComponent {
    protected:
        static int familyCounter_;
};

template<typename T>
class Component: public BaseComponent {
    public:
        static int family() {
            static int f{ familyCounter_++ };
            return f;
        }
};

}   // namespace ecs
}   // namespace ce

