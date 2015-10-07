
#pragma once

#include "ecs/Include.h"
#include "system/Events.h"
#include "app/Context.h"
#include "opengl/Program.h"

namespace pg {
namespace system {

class Render : public ecs::System<Render>, public ecs::Receiver {
    public:
        Render() = delete;
        explicit Render( Context& context );
        
        void configure( ecs::EventManager& ) override;
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;
        void receive( const PerspectiveCameraAdded& );
    
    private:
        // camera position passed as parameter
        void setSpecularUniforms_( const glm::vec3&, opengl::Program* );
        
        ecs::Entity     cameraEntity_;
        math::Matrix4f  defaultProjection_;
    
        Context& context_;
};

}   //system
}   //ce
