
#pragma once

#include "ecs/Include.h"
#include "component/Include.h"
#include "system/Events.h"
#include "app/Context.h"
#include "opengl/Program.h"

namespace pg {
namespace system {

class Renderer : public ecs::System< Renderer >, public ecs::Receiver {
    public:
        Renderer() = delete;
        explicit Renderer( Context& context );
        
        void configure( ecs::EventManager& ) override;
        void update( ecs::EntityManager&, ecs::EventManager&, float ) override;
        void receive( const ecs::ComponentAssignedEvent<component::Camera>& );
        void receive( const ecs::ComponentAssignedEvent<component::PointLight>& );

    private:
        // camera position passed as parameter
        void setSpecularUniforms_( const math::Vector3f&, opengl::Program* );
        
        // render state entities
        ecs::Entity     cameraEntity_;
        ecs::Entity     lightEntity_;   // for now the world shall have one light
        // render state math
        math::Matrix4f  defaultProjection_;

        Context& context_;
};

}   //system
}   //ce
