
#pragma once

#include "ecs/Include.h"
#include "app/Context.h"

namespace pg {
namespace system {

class Render : public ecs::System<Render> {
    public:
        Render() = delete;
        explicit Render( Context& context );
        
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;
    
    private:
        Context& context_;
};

}   //system
}   //ce
