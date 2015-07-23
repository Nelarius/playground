
#pragma once

#include <entityx/entityx.h>
#include "app/Context.h"

namespace ex = entityx;

namespace ce {
namespace system {

class Render : public ex::System<Render> {
    public:
        Render() = delete;
        explicit Render( Context& context );
        
        void update( ex::EntityManager&, ex::EventManager&, ex::TimeDelta ) override;
    
    private:
        Context& context_;
};

}   //system
}   //ce
