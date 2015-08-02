
#pragma once

#include "ecs/Include.h"
#include "app/Context.h"

namespace ce {
namespace system {

class Render : public ce::ecs::System<Render> {
    public:
        Render() = delete;
        explicit Render( Context& context );
        
        void update( ce::ecs::EntityManager&, ce::ecs::EventManager&, float ) override;
    
    private:
        Context& context_;
};

}   //system
}   //ce
