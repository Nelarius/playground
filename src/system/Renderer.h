
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
        void receive( const ToggleDebugRenderer& );

    private:
        // camera position passed as parameter
        void setSpecularUniforms_( const math::Vector3f&, opengl::Program* );
        void debugRender_( ecs::EntityManager&, ecs::EventManager&, const math::Matrix4f& cameraMatrix );

        // render state entities
        ecs::Entity     cameraEntity_;
        ecs::Entity     lightEntity_;   // for now the world shall have one light
        // render state math
        math::Matrix4f  defaultProjection_;

        Context& context_;
        bool    debug_;
};

}   //system
}   //ce
