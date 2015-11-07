
#pragma once

#include "ecs/Include.h"
#include "component/Include.h"
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
        void receive( const ecs::ComponentAssignedEvent<component::Camera>& );
        void receive( const ecs::ComponentAssignedEvent<component::PointLight>& );
    
    private:
        // camera position passed as parameter
        void setSpecularUniforms_( const math::Vector3f&, opengl::Program* );
        
        ecs::Entity     cameraEntity_;
        math::Matrix4f  defaultProjection_;
    
        Context& context_;
};

}   //system
}   //ce
