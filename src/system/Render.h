
#pragma once

#include "ecs/Include.h"
#include "app/Context.h"
#include "opengl/Program.h"

namespace pg {
namespace system {

    
class Render : public ecs::System<Render> {
    public:
        Render() = delete;
        explicit Render( Context& context );
        
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;
    
    private:
        // camera position passed as parameter
        void setSpecularUniforms_( const glm::vec3&, opengl::Program* );
    
        Context& context_;
};

}   //system
}   //ce
